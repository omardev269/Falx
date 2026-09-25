// BismIllahIRRahmaanIRRaheem
/* runtime debug definitions */
#include <runtime/rtdebug.h>
#ifdef FLX_WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif // FLX_WIN32
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <format>

#define FLX_ERROR_ASSERT 2
#define FLX_ERROR_CHECK 3
#define FLX_ERROR_OTHER 4

#ifdef FLX_DEBUG
void falx::DbgBreak()
{
#ifdef FLX_MSVC
	::__debugbreak();
#endif // FLX_MSVC
}
#endif // FLX_DEBUG

static void Fatality(const char* aProblem, const char* aTitle, uint16 aExitCode) {
#ifdef FLX_WIN32
	MessageBoxA(NULL, aProblem, aTitle, MB_OK | MB_ICONERROR);
#else
	printf("%s\n%s\n", aTitle, aProblem);
#endif // FLX_WIN32
	exit(aExitCode);
}

#ifdef FLX_DEBUG
void falx::AssertHelper(const char* aExpression, const char* aFile, uint32 aLine, const char* aFunc)
{
	DbgBreak();
	std::string m_Message = std::format("{}\n@\n{}:{}:{}", aExpression, aFile, aFunc, aLine);
	::Fatality(m_Message.c_str(), "Assertion FAILED", FLX_ERROR_ASSERT);
}
void falx::Log(const char* aMessage)
{
#ifdef FLX_WIN32
	OutputDebugStringA(aMessage);
#else
	printf("%s", aMessage);
#endif
}
#else
void falx::CheckHelper(const char* aFailure)
{
	::Fatality(aFailure, "CHECK FAILED", FLX_ERROR_CHECK);
}
#endif // FLX_DEBUG

void falx::Quit(const char* aMessage)
{
	Fatality(aMessage, "FAULT", FLX_ERROR_OTHER);
}
void falx::Quit()
{
	exit(FLX_ERROR_OTHER);
}