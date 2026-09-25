// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>
#include <runtime/rtdebug.h>
#include <runtime/time.h>
#include <runtime/window.h>

namespace falx {
	IWindow* g_iWindow;
}

#pragma region Subsystems
static FLX_INLINEFUNC void InitWindowSubsystem() {
	using namespace falx;
	CreateWindow(g_iWindow, "AlhamdullIllah!", { 800, 600 });
	FLX_CHECK(g_iWindow != nullptr, "Window creation failed");
}
static FLX_INLINEFUNC void ShutdownWindowSubsystem() {
	using namespace falx;
	g_iWindow->Dismiss();
	delete g_iWindow;
}
#pragma endregion

static FLX_INLINEFUNC void StartSubsystems() {
	using namespace falx;
	InitializeTimeInt();
	InitWindowSystem();
	InitWindowSubsystem();
}

static FLX_INLINEFUNC void ShutdownSubsystems() {
	using namespace falx;
	ShutdownWindowSubsystem();
	ShutdownWindowSystem();
}

void FalxStart() {
	using namespace falx;
	StartSubsystems();

}
bool FalxUpdate() {
	using namespace falx;
	g_iWindow->BeginFrame();
	g_iWindow->EndFrame();
	// false if the engine should stop
	return !g_iWindow->ShouldClose();
}
void FalxStop() {
	using namespace falx;
	ShutdownSubsystems();
}