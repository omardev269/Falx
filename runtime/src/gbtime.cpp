// BismIllahIRRahmaanIRRaheem
/* gb time retrieval implementation */

#include <runtime/enginedef.h>
#ifndef FLX_WIN32
#include <gb.h>
namespace falx {
	largefloat g_Initial;
	void InitializeTimeInt() {
		g_Initial = gb_time_now();
	}
	ulargeint GetTime() {
		return static_cast<ulargeint>((g_Initial - gb_time_now()) * 1000.0);
	}
	ulargeint GetTimeNotSinceFalxStartup() {
		return static_cast<ulargeint>(gb_time_now() * 1000.0);
	}
	void GetTimeBetweenAndUpdate(ulargeint& aLastTime, ulargeint& aUpdateTime) {
		largefloat m_CurrentTime = gb_time_now();
		aUpdateTime = static_cast<ulargeint>((m_CurrentTime - (static_cast<largefloat>(aLastTime) / 1000.0)) * 1000.0);
		aLastTime = static_cast<ulargeint>(m_CurrentTime * 1000.0);
	}
	void SleepMs(uint32 aMilliseconds) {
		largefloat m_Target = gb_time_now() + static_cast<largefloat>(aMilliseconds) / 1000.0;
		while (static_cast<largefloat>(gb_time_now()) <= m_Target);
	}
} 
#endif // !FLX_WIN32