// BismIllahIRRahmaanIRRaheem
/* runtime debug definitions */
#ifndef FLX_RTDEBUG_H
#define FLX_RTDEBUG_H
#include <runtime/enginedef.h>
namespace falx {
	void DbgBreak();
	void AssertHelper(const char* aExpression, const char* aFile, uint32 aLine, const char* aFunc);
	void CheckHelper(const char* aFailure);
	void Log(const char* aMessage);
	void Quit(const char* aMessage);
	void Quit();
}
#ifdef FLX_DEBUG
// Logs a message to the debug output (stdout / OutputDebugStringA) (DEBUG ONLY)
#define FLX_LOG(aMessage) ::falx::Log(aMessage)
// Breaks into the debugger if in MSVC (DEBUG ONLY)
#define FLX_BREAK() ::falx::DbgBreak()
#else
// Logs a message to the debug output (stdout / OutputDebugStringA) (DEBUG ONLY)
#define FLX_LOG(aMessage)
// Breaks into the debugger if in MSVC (DEBUG ONLY)
#define FLX_BREAK()
#endif // FLX_DEBUG
// Quits the program with a message (the message is not required to be provided)
#define FLX_QUIT(aMessage) ::falx::Quit(aMessage)
#ifdef FLX_RELEASE
// Checks a condition and quits the program with a message if the condition is false (RELEASE ONLY)
#define FLX_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::CheckHelper(aError); }
// Asserts a condition and quits the program with the failed expression if the condition is false (DEBUG ONLY)
#define FLX_ASSERT(aCondition) aCondition 
// Checks a condition and quits the program with a message if the condition is false (RELEASE ONLY)
// Asserts a condition and quits the program with the failed expression if the condition is false (DEBUG ONLY)
#define FLX_SMART_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::CheckHelper(aError); }
#else
// Checks a condition and quits the program with a message if the condition is false (RELEASE ONLY)
#define FLX_CHECK(aCondition, aError) aCondition
// Asserts a condition and quits the program with the failed expression if the condition is false (DEBUG ONLY)
#define FLX_ASSERT(aCondition) if (!(aCondition)) { ::falx::AssertHelper(#aCondition, __FILE__, __LINE__, __func__); }
// Checks a condition and quits the program with a message if the condition is false (RELEASE ONLY)
// Asserts a condition and quits the program with the failed expression if the condition is false (DEBUG ONLY)
#define FLX_SMART_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::AssertHelper(#aCondition, __FILE__, __LINE__, __func__); }
#endif // FLX_RELEASE

#endif // !FLX_RTDEBUG_H