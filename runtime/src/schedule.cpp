// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>
#include <runtime/rtdebug.h>
#include <runtime/time.h>
#include <runtime/window.h>
#include <graphics/device.h>

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
	FLX_BREAK();
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

void FalxStart() {
	using namespace falx;
	StartSubsystems();

}
bool FalxUpdate() {
	using namespace falx;
	BeginSubsystemtionalFrame();
	EndSubsystemtionalFrame();
	// false if the engine should stop
	return !g_iWindow->ShouldClose();
}
void FalxStop() {
	using namespace falx;
	ShutdownSubsystems();
}