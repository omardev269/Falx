// BismIllahIRRahmaanIRRaheem
/* win32 time getter */

#include <runtime/enginedef.h>
#ifdef FLX_WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
namespace falx {
	LARGE_INTEGER g_Frequency;
	void InitializeTime() {
		QueryPerformanceFrequency(&g_Frequency);
	};
	ulargeint GetTime() {
		LARGE_INTEGER m_CurrentTime;
		QueryPerformanceCounter(&m_CurrentTime);
#ifdef FLX_X64
		return ((m_CurrentTime.QuadPart * 1000) / g_Frequency.QuadPart);
#else
		return static_cast<ulargeint>((m_CurrentTime.QuadPart * 1000) / g_Frequency.QuadPart);
#endif // FLX_X64
	}
}
#endif // FLX_WIN32