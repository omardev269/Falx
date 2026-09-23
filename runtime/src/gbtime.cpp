// BismIllahIRRahmaanIRRaheem
/* gb time retrieval implementation */

#include <runtime/enginedef.h>
#ifndef FLX_WIN32
#include <gb.h>
namespace falx {
	void InitializeTime() {
		
	}
	ulargeint GetTime() {
		return static_cast<ulargeint>(gb_time_now() * 1000.0f);
	}
}
#endif // !FLX_WIN32