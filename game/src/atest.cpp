// BismIllahIRRahmaanIRRaheem
/* animation system test */
#if 0
#include <runtime/game.h>
#include <runtime/exposition.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <assimp/Importer.hpp>
#include <assimp/config.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <gb_math.h>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <limits>
#include <string>
#include <utility>
#include <vector>

namespace {
// This is deliberately a small CPU-skinning test, not a general model loader.
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

struct Mesh {
	int node;
	std::vector<Vertex> vertices;
	std::vector<Bone> bones;
	std::vector<uint16> indices;
	std::vector<float32> renderVertices;
	falx::ClumpID clump = {};
};

static falx::IGraphicsScene* g_Scene = nullptr;
static falx::IMaterial* g_WhiteMaterial = nullptr;
static falx::FreeflyCamera g_Camera;
static std::vector<Node> g_Nodes;
static std::vector<Channel> g_Channels;
static std::vector<Mesh> g_Meshes;
static aiMatrix4x4 g_RootInverse;
static double g_Duration = 0.0;
static double g_TicksPerSecond = 25.0;
static aiVector3D g_Center;
static float g_Scale = 1.0f;
static bool g_Ready = false;
static std::chrono::steady_clock::time_point g_StartTime;

static int FindNode(const char* name)
{
	for (size_t i = 0; i < g_Nodes.size(); ++i) {
		if (g_Nodes[i].name == name) return static_cast<int>(i);
	}
	return -1;
}

// Parents are stored before children so the global pose can be evaluated in
// one forward pass. Mesh attachments are copied separately below.
static void FlattenNode(const aiNode* node, int parent)
{
	const int index = static_cast<int>(g_Nodes.size());
	g_Nodes.push_back({ node->mName.C_Str(), parent, node->mTransformation });
	for (unsigned i = 0; i < node->mNumChildren; ++i) {
		FlattenNode(node->mChildren[i], index);
	}
}

static bool CopyMesh(const aiMesh* source, int node)
{
	// The albedo renderer uses 16-bit indices. One object per mesh avoids a
	// combined 16-bit vertex limit across otherwise independent meshes.
	if (!source || !source->HasPositions() || !source->mNumVertices ||
		source->mNumVertices > 0xffff || !source->mNumFaces ||
		source->mNumFaces > (std::numeric_limits<uint32>::max)() / 3) return false;

	Mesh mesh;
	mesh.node = node;
	mesh.vertices.resize(source->mNumVertices);
	mesh.renderVertices.resize(static_cast<size_t>(source->mNumVertices) * 5);
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
		const int boneNode = FindNode(bone->mName.C_Str());
		if (boneNode < 0) return false;
		mesh.bones.push_back({ boneNode, bone->mOffsetMatrix });
		for (unsigned j = 0; j < bone->mNumWeights; ++j) {
			const aiVertexWeight& weight = bone->mWeights[j];
			if (weight.mVertexId >= source->mNumVertices || !std::isfinite(weight.mWeight) || weight.mWeight < 0.0f) return false;
			if (weight.mWeight > 0.0f) mesh.vertices[weight.mVertexId].weights.push_back({ i, weight.mWeight });
		}
	}
	g_Meshes.push_back(std::move(mesh));
	return true;
}

// Walk attachments rather than just mMeshes: unskinned pieces need their node
// transform, and Assimp permits the same mesh to be attached more than once.
static bool CopyAttachedMeshes(const aiScene* scene, const aiNode* node)
{
	const int index = FindNode(node->mName.C_Str());
	if (index < 0) return false;
	for (unsigned i = 0; i < node->mNumMeshes; ++i) {
		if (node->mMeshes[i] >= scene->mNumMeshes || !CopyMesh(scene->mMeshes[node->mMeshes[i]], index)) return false;
	}
	for (unsigned i = 0; i < node->mNumChildren; ++i) {
		if (!CopyAttachedMeshes(scene, node->mChildren[i])) return false;
	}
	return true;
}

static bool LoadAnimation(const char* path)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	// Do not use PreTransformVertices here: it destroys the skeleton/animation.
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded);
	if (!scene || !scene->mRootNode || !scene->mNumAnimations || !scene->mAnimations[0]) {
		std::string error = std::string("Could not load an animated FBX from ") + path + ": " + importer.GetErrorString();
		FLX_LOG(error);
		MessageBoxA(nullptr, error.c_str(), "Animation test", MB_OK | MB_ICONERROR);
		return false;
	}

	FlattenNode(scene->mRootNode, -1);
	g_RootInverse = scene->mRootNode->mTransformation;
	if (std::abs(g_RootInverse.Determinant()) < 1e-8f) return false;
	g_RootInverse.Inverse();
	if (!CopyAttachedMeshes(scene, scene->mRootNode) || g_Meshes.empty()) return false;

	// Copy the first clip and all the mesh data before the importer is destroyed.
	// Its aiScene and all pointers into it cease to exist when this function ends.
	const aiAnimation* animation = scene->mAnimations[0];
	g_Duration = animation->mDuration;
	g_TicksPerSecond = animation->mTicksPerSecond > 0.0 ? animation->mTicksPerSecond : 25.0;
	for (unsigned i = 0; i < animation->mNumChannels; ++i) {
		const aiNodeAnim* source = animation->mChannels[i];
		if (!source) return false;
		Channel channel;
		channel.node = FindNode(source->mNodeName.C_Str());
		if (channel.node < 0) continue;
		if (source->mNumPositionKeys) channel.positions.assign(source->mPositionKeys, source->mPositionKeys + source->mNumPositionKeys);
		if (source->mNumRotationKeys) channel.rotations.assign(source->mRotationKeys, source->mRotationKeys + source->mNumRotationKeys);
		if (source->mNumScalingKeys) channel.scales.assign(source->mScalingKeys, source->mScalingKeys + source->mNumScalingKeys);
		g_Channels.push_back(std::move(channel));
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

static void Skin(double seconds)
{
	const double time = g_Duration > 0.0 ? std::fmod(seconds * g_TicksPerSecond, g_Duration) : 0.0;
	std::vector<aiMatrix4x4> local(g_Nodes.size());
	std::vector<aiMatrix4x4> global(g_Nodes.size());
	for (size_t i = 0; i < g_Nodes.size(); ++i) local[i] = g_Nodes[i].bind;
	for (const Channel& channel : g_Channels) {
		aiVector3D scale, position;
		aiQuaternion rotation;
		g_Nodes[channel.node].bind.Decompose(scale, rotation, position);
		local[channel.node] = aiMatrix4x4(SampleVector(channel.scales, time, scale),
			SampleRotation(channel.rotations, time, rotation), SampleVector(channel.positions, time, position));
	}
	for (size_t i = 0; i < g_Nodes.size(); ++i) {
		const int parent = g_Nodes[i].parent;
		global[i] = parent < 0 ? local[i] : global[parent] * local[i];
	}

	for (Mesh& mesh : g_Meshes) {
		std::vector<aiMatrix4x4> finalBones(mesh.bones.size());
		for (size_t i = 0; i < mesh.bones.size(); ++i) {
			// Assimp uses matrix * column-vector: mesh -> bone -> animated
			// global -> root-relative model space, in that order.
			finalBones[i] = g_RootInverse * global[mesh.bones[i].node] * mesh.bones[i].offset;
		}
		const aiMatrix4x4 rigidTransform = g_RootInverse * global[mesh.node];
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
			position = (position - g_Center) * g_Scale;
			float32* out = &mesh.renderVertices[i * 5];
			out[0] = position.x; out[1] = position.y; out[2] = position.z;
			out[3] = vertex.u; out[4] = vertex.v;
		}
	}
}

static void CreateSceneMeshes()
{
	// Fit the initial pose into a roughly two-unit box, so FBX units and model
	// origin do not make the test disappear outside the camera's clipping range.
	Skin(0.0);
	aiVector3D minPoint((std::numeric_limits<float>::max)());
	aiVector3D maxPoint(-(std::numeric_limits<float>::max)());
	for (const Mesh& mesh : g_Meshes) {
		for (size_t i = 0; i < mesh.vertices.size(); ++i) {
			const float32* p = &mesh.renderVertices[i * 5];
			minPoint.x = (std::min)(minPoint.x, p[0]); minPoint.y = (std::min)(minPoint.y, p[1]); minPoint.z = (std::min)(minPoint.z, p[2]);
			maxPoint.x = (std::max)(maxPoint.x, p[0]); maxPoint.y = (std::max)(maxPoint.y, p[1]); maxPoint.z = (std::max)(maxPoint.z, p[2]);
		}
	}
	g_Center = (minPoint + maxPoint) * 0.5f;
	const aiVector3D size = maxPoint - minPoint;
	const float extent = (std::max)(size.x, (std::max)(size.y, size.z));
	g_Scale = extent > 0.00001f ? 2.0f / extent : 1.0f;
	Skin(0.0);

	matrix4x4 identity = {};
	gb_mat4_identity(reinterpret_cast<gbMat4*>(&identity));
	for (Mesh& mesh : g_Meshes) {
		const ulargeint vertexBytes = mesh.renderVertices.size() * sizeof(float32);
		const ulargeint indexBytes = mesh.indices.size() * sizeof(uint16);
		falx::CLUMP_METADATA meta = {};
		meta.Parent = g_Scene->CreateObject(vertexBytes, indexBytes, 5 * sizeof(float32));
		meta.Transform = identity;
		meta.NumVertexBufferBytes = vertexBytes;
		meta.NumIndexBufferBytes = indexBytes;
		meta.NumVertices = static_cast<uint16>(mesh.vertices.size());
		meta.NumIndices = static_cast<uint32>(mesh.indices.size());
		// A real white material also binds a sampler. The null-material path
		// binds a white texture but does not bind its own sampler.
		meta.iMaterial = g_WhiteMaterial;
		mesh.clump = g_Scene->CreateClump(meta, mesh.renderVertices.data(), mesh.indices.data());
	}
}
}

void Game::Start()
{
	falx::CreateGraphicsScene(g_Scene, falx::g_iGdev);
	if (!g_Scene) return;
	// Visual Studio may launch from the solution root, runtime project, or test
	// directory. Try those common working directories without changing them.
	const char* paths[] = { "test/test.fbx", "../test/test.fbx", "test.fbx" };
	const char* path = paths[0];
	for (const char* candidate : paths) {
		FILE* file = nullptr;
		if (fopen_s(&file, candidate, "rb") == 0) {
			fclose(file);
			path = candidate;
			break;
		}
	}
	if (!LoadAnimation(path)) {
		MessageBoxA(nullptr, "The FBX could not be used as an animated mesh. Check its nodes, triangles and 16-bit mesh sizes.", "Animation test", MB_OK | MB_ICONERROR);
		return;
	}
	uint8 white[] = { 255, 255, 255, 255 };
	falx::MATERIAL_DESC material = {};
	material.BitmapData = white;
	material.BitmapSize = { 1, 1 };
	falx::CreateMaterial(g_WhiteMaterial, material, falx::SAMPLER_MODE_NEAREST, falx::g_iGdev);
	if (!g_WhiteMaterial) return;
	CreateSceneMeshes();
	g_Camera.Reset();
	// FreeflyCamera stores the view translation, i.e. the opposite of the
	// camera's world-space position. Its default view looks toward -Z.
	g_Camera.Teleport({ 0, 0, -5 });
	g_Camera.SetSpeed(3.0f);
	g_StartTime = std::chrono::steady_clock::now();
	g_Ready = true;
}

bit Game::PerFrame()
{
	using namespace falx;
	if (!g_Ready || g_iWindow->ShouldClose()) return false;
	const double seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - g_StartTime).count();
	Skin(seconds);
	for (Mesh& mesh : g_Meshes) {
		// UpdateClump copies this mesh's new five-float vertices into the
		// existing GPU buffer; indices and the scene allocation stay unchanged.
		g_Scene->UpdateClump(mesh.clump, mesh.renderVertices.data(), mesh.renderVertices.size() * sizeof(float32));
	}

	// Simple fixed-view navigation: WASD and space/shift move the camera.
	largefloat delta = 0;
	if (!g_iProfiler->IsFirstFrame()) g_iProfiler->GetDelta(delta);
	if (g_iWindow->GetKey(Key::W)) g_Camera.Move({ 0, 0, 1 });
	if (g_iWindow->GetKey(Key::S)) g_Camera.Move({ 0, 0, -1 });
	if (g_iWindow->GetKey(Key::A)) g_Camera.Move({ 1, 0, 0 });
	if (g_iWindow->GetKey(Key::D)) g_Camera.Move({ -1, 0, 0 });
	if (g_iWindow->GetKey(Key::Space)) g_Camera.Move({ 0, -1, 0 });
	if (g_iWindow->GetKey(Key::LeftShift)) g_Camera.Move({ 0, 1, 0 });
	g_Camera.Update(static_cast<float32>(delta), 70, 800.0f / 600.0f);
	matrix4x4 pv;
	g_Camera.GetMatrix(pv);
	g_Scene->AlbedoRender(pv);
	return true;
}

void Game::Quit()
{
	if (g_Scene) {
		g_Scene->Dismiss();
		delete g_Scene;
		g_Scene = nullptr;
	}
	if (g_WhiteMaterial) {
		g_WhiteMaterial->Dismiss();
		delete g_WhiteMaterial;
		g_WhiteMaterial = nullptr;
	}
	g_Meshes.clear();
	g_Channels.clear();
	g_Nodes.clear();
	g_Ready = false;
}
#endif