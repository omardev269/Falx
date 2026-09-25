// BismIllahIRRahmaanIRRaheem
/* window creation implementation */
#include <runtime/window.h>
#include <runtime/rtdebug.h>
#include <SDL3Window.h>

namespace falx {
	void CreateWindow(IWindow*& aWindow, const char* aTitle, int2 aWindowSize)
	{
		SDL3Window* m_Window = new SDL3Window();
		if (!m_Window->Create(aTitle, aWindowSize)) {
			FLX_ASSERT(false);
			delete m_Window;
			aWindow = nullptr;
			return;
		}
		aWindow = m_Window;
	}
}
