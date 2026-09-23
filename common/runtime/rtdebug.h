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
#define FLX_LOG(aMessage) ::falx::Log(aMessage)
#define FLX_BREAK() ::falx::DbgBreak()
#else
#define FLX_LOG(aMessage)
#define FLX_BREAK()
#endif // FLX_DEBUG
#define FLX_QUIT(aMessage) ::falx::Quit(aMessage)
#ifdef FLX_RELEASE
#define FLX_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::CheckHelper(aError); }
#define FLX_ASSERT(aCondition) aCondition 
#define FLX_SMART_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::CheckHelper(aError); }
#else
#define FLX_CHECK(aCondition, aError) aCondition
#define FLX_ASSERT(aCondition) if (!(aCondition)) { ::falx::AssertHelper(#aCondition, __FILE__, __LINE__, __func__); }
#define FLX_SMART_CHECK(aCondition, aError) if (!(aCondition)) { ::falx::AssertHelper(#aCondition, __FILE__, __LINE__, __func__); }
#endif // FLX_RELEASE

#endif // !FLX_RTDEBUG_H