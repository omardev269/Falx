// BismIllahIRRahmaanIRRaheem
/* sample game class implementation */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <graphics/loader.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <vector>

static falx::IGraphicsScene* g_iScene = nullptr;
static falx::FreeflyCamera g_Camera;

void Game::Start() {
    std::vector<falx::IndividualClump> m_Clumps;
    std::vector<falx::Brush> m_Brushes;
    falx::LoadModel("test.glb", m_Clumps, m_Brushes);
    // brushes are not supported by IGraphicsScene; release them
    for (falx::Brush& m_Brush : m_Brushes) {
        free(m_Brush.m_Bitmap);
    }
    falx::CreateGraphicsScene(g_iScene, falx::g_iGdev);
    FLX_SMART_CHECK(g_iScene != nullptr, "Scene creation failed");
    // camera
    g_Camera.Reset();
    g_Camera.Teleport({ 0.0f, 0.0f, -5.0f });
    g_Camera.Update(0.0f, 60, 800.0f / 600.0f);
    matrix4x4 m_Transform;
    g_Camera.GetMatrix(m_Transform);
    // the scene vertex format is position(3) + uv(2); all clumps share one object, so indices are offset per clump
    const uint32 c_SourceFloats = 8;
    const uint32 c_SceneFloats = 5;
    uint32 m_TotalVerts = 0;
    uint32 m_TotalIndices = 0;
    for (const falx::IndividualClump& m_Clump : m_Clumps) {
        m_TotalVerts += m_Clump.m_NumVerts;
        m_TotalIndices += m_Clump.m_NumIndices;
    }
    FLX_SMART_CHECK(m_TotalVerts > 0 && m_TotalVerts <= 0xFFFF, "Model vertex count unsupported");
    std::vector<float32> m_Verts;
    std::vector<uint16> m_Indices;
    m_Verts.reserve(static_cast<size_t>(m_TotalVerts) * c_SceneFloats);
    m_Indices.reserve(m_TotalIndices);
    std::vector<falx::CLUMP_METADATA> m_Metadata;
    for (const falx::IndividualClump& m_Clump : m_Clumps) {
        falx::CLUMP_METADATA m_Meta = {};
        m_Meta.Transform = m_Transform;
        m_Meta.StartVertexBufferBytes = m_Verts.size() * sizeof(float32);
        m_Meta.StartIndexBufferBytes = m_Indices.size() * sizeof(uint16);
        m_Meta.StartIndex = m_Indices.size();
        m_Meta.NumVertexBufferBytes = static_cast<ulargeint>(m_Clump.m_NumVerts) * c_SceneFloats * sizeof(float32);
        m_Meta.NumIndexBufferBytes = static_cast<ulargeint>(m_Clump.m_NumIndices) * sizeof(uint16);
        m_Meta.NumVertices = m_Clump.m_NumVerts;
        m_Meta.NumIndices = m_Clump.m_NumIndices;
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
    falx::ObjectID m_Object = g_iScene->CreateObject(m_Verts.size() * sizeof(float32), m_Indices.size() * sizeof(uint16), c_SceneFloats * sizeof(float32));
    for (size_t c = 0; c < m_Metadata.size(); c++) {
        m_Metadata[c].Parent = m_Object;
        g_iScene->CreateClump(m_Metadata[c],
            &m_Verts[m_Metadata[c].StartVertexBufferBytes / sizeof(float32)],
            &m_Indices[m_Metadata[c].StartIndex]);
    }
    // LoadModel allocates with new[]; the data is uploaded now
    for (falx::IndividualClump& m_Clump : m_Clumps) {
        free(m_Clump.m_ModelVerts);
        free(m_Clump.m_ModelIndices);
    }
}

bit Game::PerFrame() {
    g_iScene->AlbedoRender();
    return !falx::g_iWindow->ShouldClose();
}

void Game::Quit() {
    g_iScene->Dismiss();
    delete g_iScene;
    g_iScene = nullptr;
}
