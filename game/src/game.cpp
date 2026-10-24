// BismIllahIRRahmaanIRRaheem
/* sample game class implementation */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <graphics/loader.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <imgui.h>
#include <graphics/graphicsdef.h>
#include <gb.h>
#include <gb_math.h>
#include <vector>
struct AnimatorPhase {
	largefloat m_StartTime;
	float32 m_Duration;
	float32 m_Elapsed;
	std::string m_GuideInfo;
	float3 m_StartPos;
	float3 m_EndPos;
	float3 m_StartRotation;
	float3 m_EndRotation;
};
static std::vector<AnimatorPhase> g_AnimatorPhases = {
	{ 0, 3.5f, 0, "This is a smart classroom",{ 3, -2.5, -3 },{ 2.5, -1.5, -2.5 },{ 0, 45, 0 },{ 0, 45, 0 } },
	{ 0, 3.5f, 0, "This classroom is energy-sufficient\nIt uses solar panels!" ,{ 2.5, -5.75, -0.5 },{ 2.25, -4.75, -0.25 },{ 0, 90, 20 },{ 0, 90, 0 }, }
};
static uint32 g_AnimatorPhaseIndex = 0;
static falx::IGraphicsScene* g_iScene;
static falx::FreeflyCamera g_Camera;
static FLX_INLINEFUNC largefloat GetDeltaTime() {
#ifdef FLX_RELEASE
	largefloat m_Delta = 0;
	falx::g_iProfiler->GetDelta(m_Delta);
#else
	largefloat m_Delta = 0;
	if (!falx::g_iProfiler->IsFirstFrame()) {
		falx::g_iProfiler->GetDelta(m_Delta);
	}
#endif // FLX_RELEASE
	return m_Delta;
}
void Game::Start() {
	using namespace falx;
	g_Camera.Reset();
	CreateGraphicsScene(g_iScene, g_iGdev);
	FLX_SMART_CHECK(g_iScene != NULL, "Failed to create graphics scene");
	FLX_SMART_CHECK(LoadModelIntoScene("c2.glb", g_iScene), "Failed to load model0 into scene");
	FLX_SMART_CHECK(LoadModelIntoScene("spanel.glb", g_iScene), "Failed to load model1 into scene");
}
matrix4x4 g_PV;
bit g_Admin = false;
bit g_Free = false;
std::string g_GuideInfo;
static FLX_INLINEFUNC gbVec3 ToGbVec3(float3 aVec) {
	gbVec3 m_Result;
	m_Result.x = aVec.x;
	m_Result.y = aVec.y;
	m_Result.z = aVec.z;
	return m_Result;
}
bit Game::PerFrame() {
	using namespace falx;
	bit m_Continue = !g_iWindow->ShouldClose();
	// tourist helper
	{
		if (g_AnimatorPhaseIndex < g_AnimatorPhases.size()) {
			AnimatorPhase& m_Phase = g_AnimatorPhases[g_AnimatorPhaseIndex];
			m_Phase.m_Elapsed += static_cast<float32>(GetDeltaTime());
			if (m_Phase.m_Elapsed >= m_Phase.m_Duration) {
				g_AnimatorPhaseIndex++;
				g_GuideInfo = "";
				FLX_LOG("New phase started");
				g_Camera.Teleport({ 0,-1, 0 });
				g_Camera.SetRotation({ 0, 0, 0 });
			}
			else {
				float32 m_Timeframe = m_Phase.m_Elapsed / m_Phase.m_Duration;
				float3 m_CurrentPos;
				float3 m_CurrentRotation;
				FLX_ZMEM(&m_CurrentPos, sizeof(m_CurrentPos));
				FLX_ZMEM(&m_CurrentRotation, sizeof(m_CurrentRotation));
				gb_vec3_lerp((gbVec3*)&m_CurrentPos, ToGbVec3(m_Phase.m_StartPos), ToGbVec3(m_Phase.m_EndPos), m_Timeframe);
				gb_vec3_lerp((gbVec3*)&m_CurrentRotation, ToGbVec3(m_Phase.m_StartRotation), ToGbVec3(m_Phase.m_EndRotation), m_Timeframe);
				g_Camera.Teleport(m_CurrentPos);
				g_Camera.SetRotation(m_CurrentRotation);
				g_GuideInfo = m_Phase.m_GuideInfo;
			}
		}
		else {
			g_Free = true;
		}
	}
	// free
	{
		if (g_Free) {
			if (g_iWindow->GetKey(falx::Key::W)) {
				g_Camera.Move({ 0, 0, 1 });
			}
			if (g_iWindow->GetKey(falx::Key::S)) {
				g_Camera.Move({ 0, 0, -1 });
			}
			if (g_iWindow->GetKey(falx::Key::A)) {
				g_Camera.Move({ 1, 0, 0 });
			}
			if (g_iWindow->GetKey(falx::Key::D)) {
				g_Camera.Move({ -1, 0, 0 });
			}
			if (g_iWindow->GetKey(falx::Key::Space)) {
				g_Camera.Move({ 0, -1, 0 });
			}
			if (g_iWindow->GetKey(falx::Key::LeftShift)) {
				g_Camera.Move({ 0, 1, 0 });
			}
		}
	}
	// guidance panel
	{
		ImGui::Begin("Smart Classroom", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		if (g_Free) {
			ImGui::Text("Freemode!");
		}
		else {
			ImGui::Text(g_GuideInfo.c_str());
		}
		ImGui::End();
	}
	// admin panel
	{
		ImGui::Begin("Admin Panel");
		ImGui::Checkbox("Admin Mode", &g_Admin);
		if (g_Admin) {
			ImGui::Checkbox("Free Mode", &g_Free);
			float3 m_Position;
			float3 m_Rotation;
			g_Camera.GetPosition(m_Position);
			g_Camera.GetRotation(m_Rotation);
			ImGui::Text("Camera Position: { %.2f, %.2f, %.2f }", m_Position.x, m_Position.y, m_Position.z);
			ImGui::Text("Camera Rotation: { %.2f, %.2f, %.2f }", m_Rotation.x, m_Rotation.y, m_Rotation.z);
		}
		else {
			//g_Free = false;
		}
		ImGui::End();
	}
	// camera
	{
		g_Camera.Update(GetDeltaTime(), 70, 800.0f / 600.0f);
		g_Camera.GetMatrix(g_PV);
	}
	g_iScene->AlbedoRender(g_PV);
	return m_Continue;
}

void Game::Quit() {
	using namespace falx;
	g_iScene->Dismiss();
	delete g_iScene;
}
