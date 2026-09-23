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
		return static_cast<ulargeint>((g_Initial - gb_time_now()) * 1000.0f);
	}
	void SleepMs(uint32 aMilliseconds) {
		largefloat m_Target = gb_time_now() + static_cast<largefloat>(aMilliseconds) / 1000.0f;
		while (gb_time_now() <= m_Target);
	}
}
#endif // !FLX_WIN32