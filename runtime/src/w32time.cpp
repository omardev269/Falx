// BismIllahIRRahmaanIRRaheem
/* win32 time getter */

#include <runtime/enginedef.h>
#ifdef FLX_WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
namespace falx {
	LARGE_INTEGER g_Frequency;
	LARGE_INTEGER g_Initial;
	void InitializeTimeInt() {
		QueryPerformanceFrequency(&g_Frequency);
		QueryPerformanceCounter(&g_Initial);
	};
	ulargeint GetTime() {
		LARGE_INTEGER m_CurrentTime;
		QueryPerformanceCounter(&m_CurrentTime);
#ifdef FLX_X64
		return (((m_CurrentTime.QuadPart - g_Initial.QuadPart) * 1000) / g_Frequency.QuadPart);
#else
		return static_cast<ulargeint>(((m_CurrentTime.QuadPart - g_Initial.QuadPart) * 1000) / g_Frequency.QuadPart);
#endif // FLX_X64
	}
	void SleepMs(uint32 aMilliseconds) {
		Sleep(aMilliseconds);
	}
}
#endif // FLX_WIN32