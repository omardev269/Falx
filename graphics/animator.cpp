// BismIllahIRRahmaanIRRaheem
/* animator implementation */
#include <graphics/animator.h>
#include <graphics/loader.h>
#include <graphics/scene.h>
#include <graphics/material.h>
#include <assimp/Importer.hpp>
#include <assimp/config.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <gb_math.h>
#include <algorithm>
#include <cmath>
#include <limits>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace {
	// Deliberately a small CPU-skinning loader, not a general model loader.
	// AlbedoRender consumes exactly five floats per vertex: position followed by UV.
	struct Node {
		std::string name;
		int parent;
		aiMatrix4x4 bind;
	};

	struct Channel {
		int node;
		std::vector<aiVectorKey> positions;
		std::vector<aiQuatKey> rotations;
		std::vector<aiVectorKey> scales;
	};

	struct Weight {
		size_t bone;
		float value;
	};

	struct Vertex {
		aiVector3D position;
		float u, v;
		std::vector<Weight> weights;
	};

	struct Bone {
		int node;
		// Offsets belong to the mesh, not the skeleton: two meshes can use the
		// same node with different offsets.
		aiMatrix4x4 offset;
	};

	using ObjectHandle = decltype(std::declval<falx::IGraphicsScene*>()->CreateObject(0, 0, 0));

	struct Mesh {
		int node;
		std::vector<Vertex> vertices;
		std::vector<Bone> bones;
		std::vector<uint16> indices; // mesh-local; offset by base when combined
		size_t base = 0;             // first vertex of this mesh in the combined buffer
	};

	struct State {
		std::vector<Node> nodes;
		std::vector<Channel> channels;
		std::vector<Mesh> meshes;
		// All meshes are merged into one clump: one vertex buffer, one index buffer.
		std::vector<float32> renderVertices;
		std::vector<uint16> indices;
		falx::ClumpID clump = {};
		aiMatrix4x4 rootInverse;
		double duration = 0.0;
		double ticksPerSecond = 25.0;
		double time = 0.0; // seconds, advanced by Render(aPV, aDelta)
		aiVector3D center;
		float scale = 1.0f;
	};

	// The Animator header was not provided, so per-instance state lives here keyed by instance.
	static std::unordered_map<const falx::Animator*, State> g_States;

	static int FindNode(const State& s, const char* name)
	{
		for (size_t i = 0; i < s.nodes.size(); ++i) {
			if (s.nodes[i].name == name) return static_cast<int>(i);
		}
		return -1;
	}

	// Parents are stored before children so the global pose can be evaluated in
	// one forward pass. Mesh attachments are copied separately below.
	static void FlattenNode(State& s, const aiNode* node, int parent)
	{
		const int index = static_cast<int>(s.nodes.size());
		s.nodes.push_back({ node->mName.C_Str(), parent, node->mTransformation });
		for (unsigned i = 0; i < node->mNumChildren; ++i) {
			FlattenNode(s, node->mChildren[i], index);
		}
	}

	static bool CopyMesh(State& s, const aiMesh* source, int node)
	{
		// The albedo renderer uses 16-bit indices, so every mesh and the sum of all
		// meshes (checked in CreateSceneMeshes) must fit in 0xffff vertices.
		if (!source || !source->HasPositions() || !source->mNumVertices ||
			source->mNumVertices > 0xffff || !source->mNumFaces ||
			source->mNumFaces > (std::numeric_limits<uint32>::max)() / 3) return false;

		Mesh mesh;
		mesh.node = node;
		mesh.vertices.resize(source->mNumVertices);
		for (unsigned i = 0; i < source->mNumVertices; ++i) {
			Vertex& vertex = mesh.vertices[i];
			vertex.position = source->mVertices[i];
			vertex.u = source->HasTextureCoords(0) ? source->mTextureCoords[0][i].x : 0.0f;
			vertex.v = source->HasTextureCoords(0) ? source->mTextureCoords[0][i].y : 0.0f;
		}
		for (unsigned i = 0; i < source->mNumFaces; ++i) {
			const aiFace& face = source->mFaces[i];
			if (face.mNumIndices != 3) return false;
			for (unsigned j = 0; j < 3; ++j) {
				if (face.mIndices[j] >= source->mNumVertices) return false;
				mesh.indices.push_back(static_cast<uint16>(face.mIndices[j]));
			}
		}
		for (unsigned i = 0; i < source->mNumBones; ++i) {
			const aiBone* bone = source->mBones[i];
			if (!bone) return false;
			const int boneNode = FindNode(s, bone->mName.C_Str());
			if (boneNode < 0) return false;
			mesh.bones.push_back({ boneNode, bone->mOffsetMatrix });
			for (unsigned j = 0; j < bone->mNumWeights; ++j) {
				const aiVertexWeight& weight = bone->mWeights[j];
				if (weight.mVertexId >= source->mNumVertices || !std::isfinite(weight.mWeight) || weight.mWeight < 0.0f) return false;
				if (weight.mWeight > 0.0f) mesh.vertices[weight.mVertexId].weights.push_back({ i, weight.mWeight });
			}
		}
		s.meshes.push_back(std::move(mesh));
		return true;
	}

	// Walk attachments rather than just mMeshes: unskinned pieces need their node
	// transform, and Assimp permits the same mesh to be attached more than once.
	static bool CopyAttachedMeshes(State& s, const aiScene* scene, const aiNode* node)
	{
		const int index = FindNode(s, node->mName.C_Str());
		if (index < 0) return false;
		for (unsigned i = 0; i < node->mNumMeshes; ++i) {
			if (node->mMeshes[i] >= scene->mNumMeshes || !CopyMesh(s, scene->mMeshes[node->mMeshes[i]], index)) return false;
		}
		for (unsigned i = 0; i < node->mNumChildren; ++i) {
			if (!CopyAttachedMeshes(s, scene, node->mChildren[i])) return false;
		}
		return true;
	}

	static bool LoadAnimation(State& s, const char* path)
	{
		Assimp::Importer importer;
		importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
		// Do not use PreTransformVertices here: it destroys the skeleton/animation.
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded);
		if (!scene || !scene->mRootNode || !scene->mNumAnimations || !scene->mAnimations[0]) {
			std::string error = std::string("Could not load an animated FBX from ") + path + ": " + importer.GetErrorString();
			FLX_LOG(error);
			return false;
		}

		FlattenNode(s, scene->mRootNode, -1);
		s.rootInverse = scene->mRootNode->mTransformation;
		if (std::abs(s.rootInverse.Determinant()) < 1e-8f) return false;
		s.rootInverse.Inverse();
		if (!CopyAttachedMeshes(s, scene, scene->mRootNode) || s.meshes.empty()) return false;

		// Copy the first clip and all the mesh data before the importer is destroyed.
		// Its aiScene and all pointers into it cease to exist when this function ends.
		const aiAnimation* animation = scene->mAnimations[0];
		s.duration = animation->mDuration;
		s.ticksPerSecond = animation->mTicksPerSecond > 0.0 ? animation->mTicksPerSecond : 25.0;
		for (unsigned i = 0; i < animation->mNumChannels; ++i) {
			const aiNodeAnim* source = animation->mChannels[i];
			if (!source) return false;
			Channel channel;
			channel.node = FindNode(s, source->mNodeName.C_Str());
			if (channel.node < 0) continue;
			if (source->mNumPositionKeys) channel.positions.assign(source->mPositionKeys, source->mPositionKeys + source->mNumPositionKeys);
			if (source->mNumRotationKeys) channel.rotations.assign(source->mRotationKeys, source->mRotationKeys + source->mNumRotationKeys);
			if (source->mNumScalingKeys) channel.scales.assign(source->mScalingKeys, source->mScalingKeys + source->mNumScalingKeys);
			s.channels.push_back(std::move(channel));
		}
		return true;
	}

	// Clamp outside a key range and guard duplicate timestamps instead of dividing
	// by zero. Missing tracks retain the corresponding bind-pose component.
	static aiVector3D SampleVector(const std::vector<aiVectorKey>& keys, double time, const aiVector3D& fallback)
	{
		if (keys.empty()) return fallback;
		if (keys.size() == 1 || time <= keys.front().mTime) return keys.front().mValue;
		if (time >= keys.back().mTime) return keys.back().mValue;
		size_t i = 0;
		while (i + 1 < keys.size() - 1 && time >= keys[i + 1].mTime) ++i;
		const double span = keys[i + 1].mTime - keys[i].mTime;
		const float factor = span > 0.0 ? static_cast<float>((time - keys[i].mTime) / span) : 0.0f;
		return keys[i].mValue + (keys[i + 1].mValue - keys[i].mValue) * factor;
	}

	static aiQuaternion SampleRotation(const std::vector<aiQuatKey>& keys, double time, const aiQuaternion& fallback)
	{
		if (keys.empty()) return fallback;
		if (keys.size() == 1 || time <= keys.front().mTime) return keys.front().mValue;
		if (time >= keys.back().mTime) return keys.back().mValue;
		size_t i = 0;
		while (i + 1 < keys.size() - 1 && time >= keys[i + 1].mTime) ++i;
		const double span = keys[i + 1].mTime - keys[i].mTime;
		const float factor = span > 0.0 ? static_cast<float>((time - keys[i].mTime) / span) : 0.0f;
		aiQuaternion result;
		aiQuaternion::Interpolate(result, keys[i].mValue, keys[i + 1].mValue, factor);
		return result.Normalize();
	}

	static void Skin(State& s, double seconds)
	{
		const double time = s.duration > 0.0 ? std::fmod(seconds * s.ticksPerSecond, s.duration) : 0.0;
		std::vector<aiMatrix4x4> local(s.nodes.size());
		std::vector<aiMatrix4x4> global(s.nodes.size());
		for (size_t i = 0; i < s.nodes.size(); ++i) local[i] = s.nodes[i].bind;
		for (const Channel& channel : s.channels) {
			aiVector3D scale, position;
			aiQuaternion rotation;
			s.nodes[channel.node].bind.Decompose(scale, rotation, position);
			local[channel.node] = aiMatrix4x4(SampleVector(channel.scales, time, scale),
				SampleRotation(channel.rotations, time, rotation), SampleVector(channel.positions, time, position));
		}
		for (size_t i = 0; i < s.nodes.size(); ++i) {
			const int parent = s.nodes[i].parent;
			global[i] = parent < 0 ? local[i] : global[parent] * local[i];
		}

		for (Mesh& mesh : s.meshes) {
			std::vector<aiMatrix4x4> finalBones(mesh.bones.size());
			for (size_t i = 0; i < mesh.bones.size(); ++i) {
				// Assimp uses matrix * column-vector: mesh -> bone -> animated
				// global -> root-relative model space, in that order.
				finalBones[i] = s.rootInverse * global[mesh.bones[i].node] * mesh.bones[i].offset;
			}
			const aiMatrix4x4 rigidTransform = s.rootInverse * global[mesh.node];
			for (size_t i = 0; i < mesh.vertices.size(); ++i) {
				const Vertex& vertex = mesh.vertices[i];
				aiVector3D position(0, 0, 0);
				float total = 0.0f;
				for (const Weight& weight : vertex.weights) {
					position += (finalBones[weight.bone] * vertex.position) * weight.value;
					total += weight.value;
				}
				// Some files contain unweighted vertices. Keep them attached to their
				// mesh node; normalize nonzero sums to avoid shrinking bad weights.
				position = total > 0.0f ? position / total : rigidTransform * vertex.position;
				position = (position - s.center) * s.scale;
				float32* out = &s.renderVertices[(mesh.base + i) * 5];
				out[0] = position.x; out[1] = position.y; out[2] = position.z;
				out[3] = vertex.u; out[4] = vertex.v;
			}
		}
	}

	static bool CreateSceneMeshes(State& s, falx::IGraphicsScene* scene, ObjectHandle& object)
	{
		size_t totalVertices = 0, totalIndices = 0;
		for (Mesh& mesh : s.meshes) {
			mesh.base = totalVertices;
			totalVertices += mesh.vertices.size();
			totalIndices += mesh.indices.size();
		}
		if (totalVertices > 0xffff) return false;
		s.renderVertices.assign(totalVertices * 5, 0.0f);
		s.indices.clear();
		s.indices.reserve(totalIndices);
		for (const Mesh& mesh : s.meshes) {
			for (uint16 index : mesh.indices) s.indices.push_back(static_cast<uint16>(index + mesh.base));
		}

		// Fit the initial pose into a roughly two-unit box, so FBX units and model
		// origin do not make the test disappear outside the camera's clipping range.
		Skin(s, 0.0);
		aiVector3D minPoint((std::numeric_limits<float>::max)());
		aiVector3D maxPoint(-(std::numeric_limits<float>::max)());
		{
			for (size_t i = 0; i < totalVertices; ++i) {
				const float32* p = &s.renderVertices[i * 5];
				minPoint.x = (std::min)(minPoint.x, p[0]); minPoint.y = (std::min)(minPoint.y, p[1]); minPoint.z = (std::min)(minPoint.z, p[2]);
				maxPoint.x = (std::max)(maxPoint.x, p[0]); maxPoint.y = (std::max)(maxPoint.y, p[1]); maxPoint.z = (std::max)(maxPoint.z, p[2]);
			}
		}
		s.center = (minPoint + maxPoint) * 0.5f;
		const aiVector3D size = maxPoint - minPoint;
		const float extent = (std::max)(size.x, (std::max)(size.y, size.z));
		s.scale = extent > 0.00001f ? 2.0f / extent : 1.0f;
		Skin(s, 0.0);

		matrix4x4 identity = {};
		gb_mat4_identity(reinterpret_cast<gbMat4*>(&identity));
		// One object, one clump holding every mesh.
		const ulargeint vertexBytes = s.renderVertices.size() * sizeof(float32);
		const ulargeint indexBytes = s.indices.size() * sizeof(uint16);
		object = scene->CreateObject(vertexBytes, indexBytes, 5 * sizeof(float32));
		falx::CLUMP_METADATA meta = {};
		meta.Parent = object;
		meta.Transform = identity;
		meta.NumVertexBufferBytes = vertexBytes;
		meta.NumIndexBufferBytes = indexBytes;
		meta.NumVertices = static_cast<uint16>(totalVertices);
		meta.NumIndices = static_cast<uint32>(s.indices.size());
		// A null material makes the renderer bind its white texture.
		uint8 m_MaterialData[16] = {
			255, 255, 255, 255,
			255, 255,255, 255,
			0, 0, 0, 255,
			0, 0, 0, 255
		};
		falx::MATERIAL_DESC m_MaterialDesc = {};
		m_MaterialDesc.BitmapSize.x = 2;
		m_MaterialDesc.BitmapSize.y = 2;
		m_MaterialDesc.BitmapData = m_MaterialData;
		falx::CreateMaterial(meta.iMaterial, m_MaterialDesc, falx::SAMPLER_MODE_LINEAR, scene->GetParent());
		s.clump = scene->CreateClump(meta, s.renderVertices.data(), s.indices.data());
		return true;
	}
}

bit falx::Animator::Create(falx::IGraphicsScene* aiScene, const char* aFile)
{
	i_GraphicsScene = aiScene;

	// Original system: LoadModelIntoScene(aFile, aiScene, m_Object). The skinning loader
	// replaces it, so it is no longer called here.
	State& s = g_States[this];
	s = State();
	if (!LoadAnimation(s, aFile)) return false;

	if (!CreateSceneMeshes(s, aiScene, m_Object)) return false;
	return true;
}

void falx::Animator::Render(matrix4x4 aPV, largefloat aDelta)
{
	State& s = g_States[this];
	s.time += static_cast<double>(aDelta); // assumes aDelta is in seconds
	Skin(s, s.time);
	// UpdateClump copies the new five-float vertices into the existing GPU
	// buffer; indices and the scene allocation stay unchanged.
	i_GraphicsScene->UpdateClump(s.clump, s.renderVertices.data(), s.renderVertices.size() * sizeof(float32));
	i_GraphicsScene->AlbedoRender(m_Object, aPV);
}

void falx::Animator::Render(matrix4x4 aPV)
{
	i_GraphicsScene->AlbedoRender(m_Object, aPV);
}

falx::Animator::~Animator()
{
	i_GraphicsScene->DismissObject(m_Object);
}