// BismIllahIRRahmaanIRRaheem
/* convienience-oriented loader */

#include <graphics/loader.h>
#include <graphics/scene.h>
#include <runtime/exposition.h>
#include <runtime/enginedef.h>
#include <stdlib.h>
#include <vector>
#ifdef FLX_ACCELERATED
#include <emmintrin.h>
#endif // FLX_ACCELERATED
#include <gb.h>
#include <gb_math.h>

static bit IsBrushOpaque(const falx::Brush& aBrush)
{
	if (aBrush.m_Bitmap == NULL) return true;
	size_t m_Count = static_cast<size_t>(aBrush.m_Width) * aBrush.m_Height;
	size_t i = 0;
#ifdef FLX_ACCELERATED
	// alpha is byte 3 of each pixel: force rgb bytes to 0xFF, then all lanes must be 0xFF
	const __m128i c_ForceRGB = _mm_set1_epi32(0x00FFFFFF);
	for (; i + 4 <= m_Count; i += 4) {
		__m128i m_Pixels = _mm_loadu_si128(reinterpret_cast<const __m128i*>(&aBrush.m_Bitmap[i]));
		__m128i m_Eq = _mm_cmpeq_epi8(_mm_or_si128(m_Pixels, c_ForceRGB), _mm_set1_epi32(-1));
		if (_mm_movemask_epi8(m_Eq) != 0xFFFF) return false;
	}
#endif // FLX_ACCELERATED
	for (; i < m_Count; i++) {
		if (aBrush.m_Bitmap[i].c[3] != 255) return false;
	}
	return true;
}

namespace falx {
	static bit LoadModelIntoSceneWithTransform(const char* aPath, IGraphicsScene* aiScene, std::vector<IndividualClump>& oClumps, std::vector<Brush>& oBrushes, matrix4x4 aTransform)
{
	if (aPath == NULL || aiScene == NULL) return false;
	LoadModel(aPath, oClumps, oBrushes);
	if (oClumps.empty()) return false;

	const uint32 c_SourceFloats = 8;
	const uint32 c_SceneFloats = 5;

	std::vector<IMaterial*> m_ClumpMaterial(oClumps.size(), NULL);
	std::vector<bit> m_ClumpOpaque(oClumps.size(), true);
	for (size_t b = 0; b < oBrushes.size(); b++) {
		MATERIAL_DESC m_Desc = {};
		m_Desc.BitmapData = reinterpret_cast<uint8*>(oBrushes[b].m_Bitmap);
		m_Desc.BitmapSize.x = oBrushes[b].m_Width;
		m_Desc.BitmapSize.y = oBrushes[b].m_Height;
		IMaterial* m_Material = NULL;
		CreateMaterial(m_Material, m_Desc, SAMPLER_MODE_LINEAR, g_iGdev);
		bit m_Opaque = IsBrushOpaque(oBrushes[b]);
		for (uint32 m_Address : oBrushes[b].m_ClumpAddresses) {
			m_ClumpMaterial[m_Address] = m_Material;
			m_ClumpOpaque[m_Address] = m_Opaque;
		}
	}

	// opaque clumps first, non-opaque clumps last
	std::vector<uint32> m_Order;
	m_Order.reserve(oClumps.size());
	for (uint32 c = 0; c < oClumps.size(); c++) {
		if (m_ClumpOpaque[c]) m_Order.push_back(c);
	}
	for (uint32 c = 0; c < oClumps.size(); c++) {
		if (!m_ClumpOpaque[c]) m_Order.push_back(c);
	}

	uint32 m_TotalVerts = 0;
	uint32 m_TotalIndices = 0;
	for (const IndividualClump& m_Clump : oClumps) {
		m_TotalVerts += m_Clump.m_NumVerts;
		m_TotalIndices += m_Clump.m_NumIndices;
	}
	if (m_TotalVerts == 0 || m_TotalVerts > 0xFFFF) return false;

	std::vector<float32> m_Verts;
	std::vector<uint16> m_Indices;
	m_Verts.reserve(static_cast<size_t>(m_TotalVerts) * c_SceneFloats);
	m_Indices.reserve(m_TotalIndices);
	std::vector<CLUMP_METADATA> m_Metadata;
	m_Metadata.reserve(oClumps.size());
	
	for (uint32 m_ClumpIndex : m_Order) {
		const IndividualClump& m_Clump = oClumps[m_ClumpIndex];
		CLUMP_METADATA m_Meta = {};
		m_Meta.Transform = aTransform;
		m_Meta.StartVertexBufferBytes = m_Verts.size() * sizeof(float32);
		m_Meta.StartIndexBufferBytes = m_Indices.size() * sizeof(uint16);
		m_Meta.StartIndex = m_Indices.size();
		m_Meta.NumVertexBufferBytes = static_cast<ulargeint>(m_Clump.m_NumVerts) * c_SceneFloats * sizeof(float32);
		m_Meta.NumIndexBufferBytes = static_cast<ulargeint>(m_Clump.m_NumIndices) * sizeof(uint16);
		m_Meta.NumVertices = m_Clump.m_NumVerts;
		m_Meta.NumIndices = m_Clump.m_NumIndices;
		m_Meta.iMaterial = m_ClumpMaterial[m_ClumpIndex];
		uint16 m_VertexBase = static_cast<uint16>(m_Verts.size() / c_SceneFloats);
		for (uint32 v = 0; v < m_Clump.m_NumVerts; v++) {
			for (uint32 f = 0; f < c_SceneFloats; f++) {
				m_Verts.push_back(m_Clump.m_ModelVerts[v * c_SourceFloats + f]);
			}
		}
		for (uint32 i = 0; i < m_Clump.m_NumIndices; i++) {
			m_Indices.push_back(static_cast<uint16>(m_VertexBase + m_Clump.m_ModelIndices[i]));
		}
		m_Metadata.push_back(m_Meta);
	}

	ObjectID m_NewObject = aiScene->CreateObject(m_Verts.size() * sizeof(float32), m_Indices.size() * sizeof(uint16), c_SceneFloats * sizeof(float32));
	for (CLUMP_METADATA& m_Meta : m_Metadata) {
		m_Meta.Parent = m_NewObject;
		aiScene->CreateClump(m_Meta,
			&m_Verts[m_Meta.StartVertexBufferBytes / sizeof(float32)],
			&m_Indices[m_Meta.StartIndex]);
	}
	return true;
}
}

bit falx::LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene, std::vector<IndividualClump>& oClumps, std::vector<Brush>& oBrushes)
{
	matrix4x4 m_Identity = {};
	gb_mat4_identity((gbMat4*)&m_Identity);
	return LoadModelIntoSceneWithTransform(aPath, aiScene, oClumps, oBrushes, m_Identity);
}

bit falx::LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene)
{
	std::vector<IndividualClump> m_Clumps;
	std::vector<Brush> m_Brushes;
	bit m_Result = LoadModelIntoScene(aPath, aiScene, m_Clumps, m_Brushes);
	for (IndividualClump& m_Clump : m_Clumps) {
		free(m_Clump.m_ModelVerts);
		free(m_Clump.m_ModelIndices);
	}
	for (Brush& m_Brush : m_Brushes) {
		free(m_Brush.m_Bitmap);
	}
	return m_Result;
}

bit falx::LoadModelIntoScene(const char* aPath, IGraphicsScene* aiScene, float3 aPosition, float3 aRotation)
{
	if (aPath == NULL || aiScene == NULL) return false;

	gbMat4 m_RotateX;
	gbMat4 m_RotateY;
	gbMat4 m_RotateZ;
	gbMat4 m_RotateXY;
	gbMat4 m_Rotation;
	gbMat4 m_Translation;
	gbMat4 m_Transform;
	gb_mat4_rotate(&m_RotateX, { 1, 0, 0 }, gb_to_radians(aRotation.x));
	gb_mat4_rotate(&m_RotateY, { 0, 1, 0 }, gb_to_radians(aRotation.y));
	gb_mat4_rotate(&m_RotateZ, { 0, 0, 1 }, gb_to_radians(aRotation.z));
	gb_mat4_mul(&m_RotateXY, &m_RotateX, &m_RotateY);
	gb_mat4_mul(&m_Rotation, &m_RotateXY, &m_RotateZ);
	gb_mat4_translate(&m_Translation, { aPosition.x, aPosition.y, aPosition.z });
	gb_mat4_mul(&m_Transform, &m_Rotation, &m_Translation);

	std::vector<IndividualClump> m_Clumps;
	std::vector<Brush> m_Brushes;
	bit m_Result = LoadModelIntoSceneWithTransform(aPath, aiScene, m_Clumps, m_Brushes, *reinterpret_cast<matrix4x4*>(&m_Transform));
	for (IndividualClump& m_Clump : m_Clumps) {
		free(m_Clump.m_ModelVerts);
		free(m_Clump.m_ModelIndices);
	}
	for (Brush& m_Brush : m_Brushes) {
		free(m_Brush.m_Bitmap);
	}
	return m_Result;
}

