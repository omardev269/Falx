// BismIllahIRRahmaanIRRaheem
/* declaration for time-related features */

#ifndef FLX_TIME_H
#define FLX_TIME_H
#include <runtime/enginedef.h>
namespace falx
{
	// Get the time in milliseconds (fractional) since the engine started
	largefloat GetTime();
	// Gets the time in milliseconds (fractional) the way the system says it (maybe since the system started, maybe since the unix epoch, et cetera)
	largefloat GetTimeNotSinceFalxStartup();
	// To be used for measuring time between two points, and updating the last time with the current time (both in milliseconds, as returned by GetTimeNotSinceFalxStartup)
	void GetTimeBetweenAndUpdate(largefloat& aLastTime, largefloat& aUpdateTime);
	// Halt thread for a given number of milliseconds
	void SleepMs(uint32 aMilliseconds);
}
#endif // !FLX_TIME_H