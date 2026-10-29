// BismIllahIRRahmaanIRRaheem
/* gb time retrieval implementation */

#include <runtime/enginedef.h>
#ifndef FLX_WIN32
#include <gb.h>
#include <threads.h>
#include <time.h>
namespace falx {
	static largefloat g_Initial;
	void InitializeTimeInt() {
		g_Initial = static_cast<largefloat>(gb_time_now());
	}
	largefloat GetTime() {
		return (static_cast<largefloat>(gb_time_now()) - g_Initial);
	}
	largefloat GetTimeNotSinceFalxStartup() {
		return static_cast<largefloat>(gb_time_now());
	}
	void GetTimeBetweenAndUpdate(largefloat& aLastTime, largefloat& aUpdateTime) {
		largefloat m_CurrentTime = GetTimeNotSinceFalxStartup();
		aUpdateTime = m_CurrentTime - aLastTime;
		aLastTime = m_CurrentTime;
	}
	void SleepSeconds(largefloat aSeconds) {
		if (aSeconds <= 0) return;
		struct timespec m_Duration;
		m_Duration.tv_sec = static_cast<time_t>(aSeconds);
		m_Duration.tv_nsec = static_cast<long>((aSeconds - static_cast<largefloat>(m_Duration.tv_sec)) * 1000000000.0);
		while (thrd_sleep(&m_Duration, &m_Duration) == -1);
	}
} 
#endif // !FLX_WIN32