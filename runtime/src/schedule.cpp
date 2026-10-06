// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>
#include <runtime/rtdebug.h>
#include <runtime/time.h>
#include <runtime/window.h>
#include <graphics/device.h>
#include <graphics/scene.h>
#include <gb_math.h>

namespace falx {
	inline IWindow* g_iWindow;
	inline IGraphicsDevice* g_iGdev;
}

#pragma region Subsystems
// -

static FLX_INLINEFUNC void InitWindowSubsystem() {
	using namespace falx;
	CreateWindowNW(g_iWindow, "AlhamdullIllah!", { 800, 600 });
	FLX_SMART_CHECK(g_iWindow != nullptr, "Window creation failed");
}
static FLX_INLINEFUNC void InitRenderingSubsystem() {
	using namespace falx;
#ifdef FLX_WIN32
#ifdef FLX_DEBUG
#ifdef FLX_MSVC
	system("pause"); // hook into renderdoc
#endif // FLX_MSVC
#endif // FLX_DEBUG
#endif // FLX_WIN32
	GetGraphicsAPI();
	CreateGraphicsDevice(g_iGdev, g_iWindow);
	FLX_SMART_CHECK(g_iGdev != nullptr, "Gdev creation failed");
}
static FLX_INLINEFUNC void BeginWindowFrame() {
	using namespace falx;
	g_iWindow->BeginFrame();
}
static FLX_INLINEFUNC void EndWindowFrame() {
	using namespace falx;
	
	g_iWindow->EndFrame();
}
static FLX_INLINEFUNC void BeginRenderingFrame() {
	using namespace falx;
	g_iGdev->BeginFrame();
}
static FLX_INLINEFUNC void EndRenderingFrame() {
	using namespace falx;
	g_iGdev->EndFrame();
}
static FLX_INLINEFUNC void ShutdownWindowSubsystem() {
	using namespace falx;
	g_iWindow->Dismiss();
	delete g_iWindow;
}
static FLX_INLINEFUNC void ShutdownRenderingSubsystem() {
	using namespace falx;
	g_iGdev->Dismiss();
	delete g_iGdev;
}
#pragma endregion

static FLX_INLINEFUNC void StartSubsystems() {
	using namespace falx;
	InitializeTimeInt();
	InitWindowSystem();
	InitWindowSubsystem();
	InitRenderingSubsystem();
}

static FLX_INLINEFUNC void BeginSubsystemtionalFrame() {
	using namespace falx;
	BeginWindowFrame();
	BeginRenderingFrame();
}

static FLX_INLINEFUNC void EndSubsystemtionalFrame() {
	using namespace falx;
	EndRenderingFrame();
	EndWindowFrame();
}

static FLX_INLINEFUNC void ShutdownSubsystems() {
	using namespace falx;
	ShutdownWindowSubsystem();
	ShutdownRenderingSubsystem();
	ShutdownWindowSystem();
}

falx::IGraphicsScene* i_GraphicsScene;
matrix4x4 m_Mat;
falx::ObjectID m_ObjectID;
falx::ClumpID m_ClumpID;
void FalxStart() {
	using namespace falx;
	StartSubsystems();
	gb_mat4_identity((gbMat4*)&m_Mat);
	CreateGraphicsScene(i_GraphicsScene, g_iGdev);
	FLX_SMART_CHECK(i_GraphicsScene != NULL, "Test graphics scene creation failed");
	m_ObjectID = i_GraphicsScene->CreateObject(60, 6, 20);
	CLUMP_METADATA m_Metadata;
	FLX_ZMEM(&m_Metadata, sizeof(CLUMP_METADATA));
	m_Metadata.Parent = m_ObjectID;
	m_Metadata.Transform = m_Mat;
	m_Metadata.StartVertexBufferBytes = 0;
	m_Metadata.StartIndexBufferBytes = 0;
	m_Metadata.StartIndex = 0;
	m_Metadata.NumVertexBufferBytes = 60;
	m_Metadata.NumIndexBufferBytes = 6;
	m_Metadata.NumVertices = 3;
	m_Metadata.NumIndices = 3;
	// -
	float32 m_TriangleVerts[15] = {
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f, 1.0f, 1.0f
	};
	uint16 m_TriangleIndices[3] = { 0, 1, 2 };
	m_ClumpID = i_GraphicsScene->CreateClump(m_Metadata, m_TriangleVerts, m_TriangleIndices);
}
bool FalxUpdate() {
	using namespace falx;
	BeginSubsystemtionalFrame();
	i_GraphicsScene->AlbedoRender();
	EndSubsystemtionalFrame();
	// false if the engine should stop
	return !g_iWindow->ShouldClose();
}
void FalxStop() {
	using namespace falx;
	i_GraphicsScene->DismissClump(m_ClumpID);
	i_GraphicsScene->DismissObject(m_ObjectID);
	i_GraphicsScene->Dismiss();
	delete i_GraphicsScene;
	ShutdownSubsystems();
}