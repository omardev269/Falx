// BismIllahIRRahmaanIRRaheem
/* declaration for time-related features */

#ifndef FLX_TIME_H
#define FLX_TIME_H
#include <runtime/enginedef.h>
namespace falx
{
	// Get the time in seconds (fractional) since the engine started
	largefloat GetTime();
	// Gets the time in seconds (fractional) the way the system says it (maybe since the system started, maybe since the unix epoch, et cetera)
	largefloat GetTimeNotSinceFalxStartup();
	// To be used for measuring time between two points, and updating the last time with the current time (both in seconds, as returned by GetTimeNotSinceFalxStartup)
	void GetTimeBetweenAndUpdate(largefloat& aLastTime, largefloat& aUpdateTime);
	// Halt thread for a given number of seconds (fractional)
	void SleepSeconds(largefloat aSeconds);
}
#endif // !FLX_TIME_H