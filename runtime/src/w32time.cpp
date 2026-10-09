// BismIllahIRRahmaanIRRaheem
/* win32 time getter */

#include <runtime/enginedef.h>
#ifdef FLX_WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <runtime/rtdebug.h>
namespace falx {
	static LARGE_INTEGER g_Frequency;
	static LARGE_INTEGER g_Initial;
	void InitializeTimeInt() {
		FLX_SMART_CHECK(QueryPerformanceFrequency(&g_Frequency), "QueryPerformanceFrequency Failed\nAre you sure you are on a version of Microsoft Windows >= XP?");
		FLX_SMART_CHECK(QueryPerformanceCounter(&g_Initial), "QueryPerformanceCounter Failed\nAre you sure you are on a version of Microsoft Windows >= XP?");
	};
	// converts performance counter ticks to seconds
	static largefloat TicksToSeconds(LONGLONG aTicks) {
		return static_cast<largefloat>(aTicks) / static_cast<largefloat>(g_Frequency.QuadPart);
	}
	largefloat GetTime() {
		LARGE_INTEGER m_CurrentTime;
		// it is not supposed to fault because the initializer didnt fault
		QueryPerformanceCounter(&m_CurrentTime);
		return TicksToSeconds(m_CurrentTime.QuadPart - g_Initial.QuadPart);
	}
	largefloat GetTimeNotSinceFalxStartup() {
		LARGE_INTEGER m_CurrentTime;
		// it is not supposed to fault because the initializer didnt fault
		QueryPerformanceCounter(&m_CurrentTime);
		return TicksToSeconds(m_CurrentTime.QuadPart);
	};
	void GetTimeBetweenAndUpdate(largefloat& aLastTime, largefloat& aUpdateTime) {
		largefloat m_CurrentTime = GetTimeNotSinceFalxStartup();
		aUpdateTime = m_CurrentTime - aLastTime;
		aLastTime = m_CurrentTime;
	}
	void SleepSeconds(largefloat aSeconds) {
		// Sleep only takes 32-bit unsigned integers (milliseconds)
		Sleep(static_cast<DWORD>(aSeconds * static_cast<largefloat>(1000)));
	}
}
#endif // FLX_WIN32