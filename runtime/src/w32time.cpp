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
	ulargeint GetTime() {
		LARGE_INTEGER m_CurrentTime;
		// it is not supposed to fault because the initializer didnt fault
		QueryPerformanceCounter(&m_CurrentTime);
#ifdef FLX_X64
		return (((m_CurrentTime.QuadPart - g_Initial.QuadPart) * 1000) / g_Frequency.QuadPart);
#else
		return static_cast<ulargeint>(((m_CurrentTime.QuadPart - g_Initial.QuadPart) * 1000) / g_Frequency.QuadPart);
#endif // FLX_X64
	}
	ulargeint GetTimeNotSinceFalxStartup() {
		LARGE_INTEGER m_CurrentTime;
		// it is not supposed to fault because the initializer didnt fault
		QueryPerformanceCounter(&m_CurrentTime);
#ifdef FLX_X64
		return (((m_CurrentTime.QuadPart) * 1000) / g_Frequency.QuadPart);
#else
		return static_cast<ulargeint>(((m_CurrentTime.QuadPart) * 1000) / g_Frequency.QuadPart);
#endif // FLX_X64
	};
	void GetTimeBetweenAndUpdate(ulargeint& aLastTime, ulargeint& aUpdateTime) {
		LARGE_INTEGER m_CurrentTime;
		// it is not supposed to fault because the initializer didnt fault
		QueryPerformanceCounter(&m_CurrentTime);
#ifdef FLX_X64
		aUpdateTime = (((m_CurrentTime.QuadPart - aLastTime) * 1000) / g_Frequency.QuadPart);
		aLastTime = (((m_CurrentTime.QuadPart) * 1000) / g_Frequency.QuadPart);
#else
		aUpdateTime = static_cast<ulargeint>(((m_CurrentTime.QuadPart - static_cast<LONGLONG>(aLastTime)) * 1000) / g_Frequency.QuadPart);
		aLastTime = static_cast<ulargeint>(((m_CurrentTime.QuadPart) * 1000) / g_Frequency.QuadPart);
#endif // FLX_X64
	}
	void SleepMs(uint32 aMilliseconds /* Sleep only takes 32-bit unsigned integers */) {
		Sleep(aMilliseconds);
	}
}
#endif // FLX_WIN32