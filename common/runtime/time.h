// BismIllahIRRahmaanIRRaheem
/* declaration for time-related features */

#ifndef FLX_TIME_H
#define FLX_TIME_H
#include <runtime/enginedef.h>
namespace falx
{
	// Get the time in milliseconds since the engine started
	ulargeint GetTime();
	// Gets the time in milliseconds the way the system says it (maybe since the system started, maybe since the unix epoch, et cetera)
	ulargeint GetTimeNotSinceFalxStartup();
	// To be used for measuring time between two points, and updating the last time with the current time
	void GetTimeBetweenAndUpdate(ulargeint& aLastTime, ulargeint& aUpdateTime);
	// Halt thread for a given number of milliseconds
	void SleepMs(uint32 aMilliseconds);
}
#endif // !FLX_TIME_H