// BismIllahIRRahmaanIRRaheem
/* gb time retrieval implementation */

#include <runtime/enginedef.h>
#ifndef FLX_WIN32
#include <gb.h>
namespace falx {
	static largefloat g_Initial;
	void InitializeTimeInt() {
		g_Initial = static_cast<largefloat>(gb_time_now());
	}
	largefloat GetTime() {
		return (static_cast<largefloat>(gb_time_now()) - g_Initial) * static_cast<largefloat>(1000);
	}
	largefloat GetTimeNotSinceFalxStartup() {
		return static_cast<largefloat>(gb_time_now()) * static_cast<largefloat>(1000);
	}
	void GetTimeBetweenAndUpdate(largefloat& aLastTime, largefloat& aUpdateTime) {
		largefloat m_CurrentTime = GetTimeNotSinceFalxStartup();
		aUpdateTime = m_CurrentTime - aLastTime;
		aLastTime = m_CurrentTime;
	}
	void SleepMs(uint32 aMilliseconds) {
		largefloat m_Target = static_cast<largefloat>(gb_time_now()) + static_cast<largefloat>(aMilliseconds) / static_cast<largefloat>(1000);
		while (static_cast<largefloat>(gb_time_now()) <= m_Target);
	}
} 
#endif // !FLX_WIN32