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
	{ 0, 3.5f, 0, "This classroom is energy-sufficient\nIt uses solar panels!" ,{ 2.5, -5.75, -0.5 },{ 2.25, -4.75, -0.25 },{ 0, 90, 20 },{ 0, 90, 0 }, },
	{ 0, 4.0f, 0, "For the comfort of the students\nThis classroom is equipped with a modern AC\nIt works only when the temperature is\nless than 20(c) or higher than 22(c)",{ 1.5, -2.45, -2.0 },{ 2, -2.65, -2 },{ 0, 270, 15 },{ 0,270,0 } }
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
	FLX_SMART_CHECK(LoadModelIntoScene("ac.glb", g_iScene, { -3,2.6,1.75 }, { 0,90,0 }), "Failed to load model2 into scene");
}
matrix4x4 g_PV;
bit g_Admin = false;
bit g_Free = false;
std::string g_GuideInfo;
static FLX_INLINEFUNC gbVec3 ToGbVec3(float3 aVec) {
	return { aVec.x, aVec.y, aVec.z };
}
bit Game::PerFrame() {
	using namespace falx;
	bit m_Continue = !g_iWindow->ShouldClose();
	// concentration switch (made initially by AI)
	{
		static bit s_Concentrated = true;
		static bit s_EscWasDown = false;
		bit m_EscDown = g_iWindow->GetKey(falx::Key::Escape) || g_iWindow->GetKeyUI(falx::Key::Escape);
		if (m_EscDown && !s_EscWasDown) {
			s_Concentrated = !s_Concentrated;
			g_iWindow->ConcentrateMouse(s_Concentrated);
		}
		s_EscWasDown = m_EscDown;
	}
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
			float3 m_CamRotation;
			g_Camera.GetRotation(m_CamRotation);
			const float32 m_YawRad = gb_to_radians(m_CamRotation.y);
			const float32 m_Cos = cosf(m_YawRad);
			const float32 m_Sin = sinf(m_YawRad);
			if (g_iWindow->GetKey(falx::Key::W)) {
				g_Camera.Move({ -m_Sin, 0, m_Cos });
			}
			if (g_iWindow->GetKey(falx::Key::S)) {
				g_Camera.Move({ m_Sin, 0, -m_Cos });
			}
			if (g_iWindow->GetKey(falx::Key::A)) {
				g_Camera.Move({ m_Cos, 0, m_Sin });
			}
			if (g_iWindow->GetKey(falx::Key::D)) {
				g_Camera.Move({ -m_Cos, 0, -m_Sin });
			}
			if (g_iWindow->GetKey(falx::Key::Space)) {
				g_Camera.Move({ 0, -1, 0 });
			}
			if (g_iWindow->GetKey(falx::Key::LeftShift)) {
				g_Camera.Move({ 0, 1, 0 });
			}
			// view rotation (skipped on the first frame)
			static bit s_FirstLookFrame = true;
			float2 m_MouseDelta;
			g_iWindow->GetMouseDelta(m_MouseDelta);
			if (s_FirstLookFrame) {
				s_FirstLookFrame = false;
			}
			else {
				const float32 k_Sensitivity = 0.1f;
				g_Camera.Rotate({ m_MouseDelta.y * k_Sensitivity, m_MouseDelta.x * k_Sensitivity, 0 });
			}
		}
	}
	// guidance panel
	{
		ImGui::Begin("Smart Classroom", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_AlwaysAutoResize);
		if (g_Free) {
			ImGui::Text("Freemode!\nUse WASD for movement and space + \nLeft Shift for vertical movement\nUse the mouse for rotating the camera");
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
