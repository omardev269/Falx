// BismIllahIRRahmaanIRRaheem
/* sample game class implementation */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <graphics/loader.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <imgui.h>
#include <graphics/graphicsdef.h>
#include <vector>

static falx::IGraphicsScene* g_iScene = nullptr;
static falx::FreeflyCamera g_Camera;
static falx::ObjectID g_ModelObject = {};
static bit g_HasModel = false;
uint32 g_NumClumps = 0;
std::vector<falx::ClumpID> g_Clumps;
std::vector<falx::IMaterial*> g_Materials;
uint32 g_WantedClump = 0;
char* g_FilePath;
void Game::Start() {
	g_FilePath = (char*)malloc(260);
	FLX_SMART_CHECK(g_FilePath != NULL, "File path allocation failed");
	g_FilePath[0] = '\0';
	falx::CreateGraphicsScene(g_iScene, falx::g_iGdev);
	FLX_SMART_CHECK(g_iScene != NULL, "Scene creation failed");
	g_Camera.Reset();
	g_Camera.Teleport({ 0.0f, 0.0f, -5.0f });
	g_Camera.Update(0.0f, 60, 800.0f / 600.0f);
}

bit g_ShouldOpenSelector = false;
bit g_ShouldAllDraw = true;
bit Game::PerFrame() {
	bit m_Return = !falx::g_iWindow->ShouldClose();
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Open")) {
				g_ShouldOpenSelector = true;
			}
			if (ImGui::MenuItem("Exit")) {
				m_Return = false;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (g_ShouldOpenSelector) {
		ImGui::Begin("Open File", &g_ShouldOpenSelector, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::InputText("File Path", g_FilePath, 260);
	  if (ImGui::Button("Load") && g_FilePath[0] != '\0') {
			if (g_HasModel) {
				for (falx::ClumpID& m_Clump : g_Clumps) {
					g_iScene->DismissClump(m_Clump);
				}
				g_iScene->DismissObject(g_ModelObject);
				for (falx::IMaterial* m_Material : g_Materials) {
					if (m_Material == NULL) continue;
					m_Material->Dismiss();
					delete m_Material;
				}
				g_Materials.clear();
				g_Clumps.clear();
				g_NumClumps = 0;
				g_WantedClump = 0;
				g_HasModel = false;
			}
			std::vector<falx::IndividualClump> m_Clumps;
			std::vector<falx::Brush> m_Brushes;
			falx::LoadModel(g_FilePath, m_Clumps, m_Brushes);
			std::vector<falx::IMaterial*> m_Materials;
			for (falx::Brush& m_Brush : m_Brushes) {
				falx::MATERIAL_DESC m_Desc = {};
				m_Desc.BitmapData = reinterpret_cast<uint8*>(m_Brush.m_Bitmap);
				m_Desc.BitmapSize.x = m_Brush.m_Width;
				m_Desc.BitmapSize.y = m_Brush.m_Height;
				falx::IMaterial* m_Material = NULL;
				falx::CreateMaterial(m_Material, m_Desc, falx::SAMPLER_MODE_LINEAR, falx::g_iGdev);
				m_Materials.push_back(m_Material);
				free(m_Brush.m_Bitmap);
			}
			uint32 m_TotalVerts = 0;
			uint32 m_TotalIndices = 0;
			for (const falx::IndividualClump& m_Clump : m_Clumps) {
				m_TotalVerts += m_Clump.m_NumVerts;
				m_TotalIndices += m_Clump.m_NumIndices;
			}
			if (!m_Clumps.empty() && m_TotalVerts > 0 && m_TotalVerts <= 0xFFFF) {
				const uint32 c_SourceFloats = 8;
				const uint32 c_SceneFloats = 5;
				std::vector<float32> m_Verts;
				std::vector<uint16> m_Indices;
				m_Verts.reserve(static_cast<size_t>(m_TotalVerts) * c_SceneFloats);
				m_Indices.reserve(m_TotalIndices);
				std::vector<falx::CLUMP_METADATA> m_Metadata;
				matrix4x4 m_Transform;
				g_Camera.GetMatrix(m_Transform);
				uint32 m_ClumpIndicator = 0;
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
					m_ClumpIndicator++;
				}
				for (size_t b = 0; b < m_Brushes.size(); b++) {
					for (uint32 m_Address : m_Brushes[b].m_ClumpAddresses) {
						m_Metadata[m_Address].iMaterial = m_Materials[b];
					}
				}
				falx::ObjectID m_NewObject = g_iScene->CreateObject(m_Verts.size() * sizeof(float32), m_Indices.size() * sizeof(uint16), c_SceneFloats * sizeof(float32));
				std::vector<falx::ClumpID> m_NewClumps;
				for (size_t c = 0; c < m_Metadata.size(); c++) {
					m_Metadata[c].Parent = m_NewObject;
					m_NewClumps.push_back(g_iScene->CreateClump(m_Metadata[c],
						&m_Verts[m_Metadata[c].StartVertexBufferBytes / sizeof(float32)],
						&m_Indices[m_Metadata[c].StartIndex]));
				}
				g_ModelObject = m_NewObject;
				g_Clumps.swap(m_NewClumps);
				g_Materials.swap(m_Materials);
				g_NumClumps = static_cast<uint32>(g_Clumps.size());
				g_WantedClump = 0;
				g_HasModel = true;
				g_ShouldOpenSelector = false;
			}
		}
		ImGui::End();
	}
#ifdef FLX_ALLOW_IMGUI
	if (g_HasModel) {
		ImGui::Begin("AlhamdullIllah!");
		if (ImGui::Button("Toggle All Draw")) {
			g_ShouldAllDraw = !g_ShouldAllDraw;
		}
		if (!g_ShouldAllDraw) {
			ImGui::SliderInt("Select Clump", reinterpret_cast<int*>(&g_WantedClump), 0, static_cast<int>(g_NumClumps) - 1);
		}
		ImGui::End();
	}
#endif // FLX_ALLOW_IMGUI
	if (falx::g_iWindow->GetKey(falx::Key::W)) {
		g_Camera.Move({ 0.0f, 0.0f, 1.0f });
	}
	if (falx::g_iWindow->GetKey(falx::Key::S)) {
		g_Camera.Move({ 0.0f, 0.0f, -1.0f });
	}
	if (falx::g_iWindow->GetKey(falx::Key::D)) {
		g_Camera.Move({ -1.0f, 0.0f, 0.0f });
	}
	if (falx::g_iWindow->GetKey(falx::Key::A)) {
		g_Camera.Move({ 1.0f, 0.0f, 0.0f });
	}
	if (falx::g_iWindow->GetKey(falx::Key::Space)) {
		g_Camera.Move({ 0.0f, -1.0f, 0.0f });
	}
	if (falx::g_iWindow->GetKey(falx::Key::LeftShift)) {
		g_Camera.Move({ 0.0f, 1.0f, 0.0f });
	}
	largefloat m_DeltaTime = 0;
	if (!falx::g_iProfiler->IsFirstFrame()) {
		falx::g_iProfiler->GetDelta(m_DeltaTime);
	}
	g_Camera.Update(m_DeltaTime, 60, 800.0f / 600.0f);
	matrix4x4 m_Transform;
	g_Camera.GetMatrix(m_Transform);
	for (size_t c = 0; c < g_Clumps.size(); c++) {
		g_iScene->UpdateClump(g_Clumps[c], m_Transform);
	}
	if (g_ShouldAllDraw) {
		g_iScene->AlbedoRender();
	}
	else {
		if (g_HasModel && g_WantedClump < g_Clumps.size()) {
			g_iScene->AlbedoRender(g_Clumps[g_WantedClump]);
		}
	}
	return m_Return;
}

void Game::Quit() {
	for (falx::IMaterial* m_Material : g_Materials) {
		if (((ulargeint)m_Material) != NULL) {
			m_Material->Dismiss();
			delete m_Material;
		}
	}
	g_Materials.clear();
	free((void*)g_FilePath);
	g_iScene->Dismiss();
	delete g_iScene;
	g_iScene = NULL;
}
