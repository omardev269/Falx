// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>
#include <runtime/rtdebug.h>
#include <runtime/time.h>
#include <runtime/window.h>
#include <graphics/device.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <runtime/profiler.h>
#include <gb_math.h>
#include <vector>
#include <runtime/game.h>
#include <runtime/exposition.h>

namespace falx {
	Game g_Game;
}

#pragma region Subsystems
// -

static FLX_INLINEFUNC void InitWindowSubsystem() {
	using namespace falx;
	CreateWindowNW(g_iWindow, "AlhamdullIllah!", { 800, 600 });
	FLX_SMART_CHECK(g_iWindow != nullptr, "Window creation failed");
	g_IUpdateables.push_back(g_iWindow);
}
static FLX_INLINEFUNC void InitRenderingSubsystem() {
	using namespace falx;
#ifdef FLX_WIN32
#ifdef FLX_DEBUG
#ifdef FLX_MSVC
	system("echo Hook into RenderDoc && pause"); // hook into renderdoc
#endif // FLX_MSVC
#endif // FLX_DEBUG
#endif // FLX_WIN32
	GetGraphicsAPI();
	CreateGraphicsDevice(g_iGdev, g_iWindow);
	FLX_SMART_CHECK(g_iGdev != nullptr, "Gdev creation failed");
	g_IUpdateables.push_back(g_iGdev);
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
static FLX_INLINEFUNC void InitProfiler() {
	using namespace falx;
	CreateProfiler(g_iProfiler);
	FLX_SMART_CHECK(g_iProfiler != NULL, "Profiler creation failed");
	g_iProfiler->Initialize();
	g_IUpdateables.push_back(g_iProfiler);
}
static FLX_INLINEFUNC void ShutdownProfiler() {
	using namespace falx;
	g_iProfiler->Dismiss();
	delete g_iProfiler;
}
#pragma endregion

static FLX_INLINEFUNC void StartSubsystems() {
	using namespace falx;
	InitializeTimeInt();
	InitProfiler();
	InitWindowSystem();
	InitWindowSubsystem();
	InitRenderingSubsystem();
}

static FLX_INLINEFUNC void BeginSubsystemtionalFrame() {
	using namespace falx;
	for (size_t i = 0; i < g_IUpdateables.size(); i++) {
		g_IUpdateables[i]->BeginFrame();
	}
}

static FLX_INLINEFUNC void EndSubsystemtionalFrame() {
	using namespace falx;
	for (size_t i = g_IUpdateables.size(); i > 0; i--) {
		g_IUpdateables[i - 1]->EndFrame();
	}
}

static FLX_INLINEFUNC void ShutdownSubsystems() {
	using namespace falx;
	ShutdownRenderingSubsystem();
	ShutdownWindowSubsystem();
	ShutdownWindowSystem();
	ShutdownProfiler();
}

void FalxStart() {
	using namespace falx;
	StartSubsystems();
	g_Game.Start();
}
bool FalxUpdate() {
	using namespace falx;
	BeginSubsystemtionalFrame();
	// false if the engine should stop; runs before the frame ends so the game can render into it
	bit m_ShouldContinue = g_Game.PerFrame();
	EndSubsystemtionalFrame();
	return m_ShouldContinue;
}
void FalxStop() {
	using namespace falx;
	g_Game.Quit();
	ShutdownSubsystems();
}