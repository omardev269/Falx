// BismIllahIRRahmaanIRRaheem
/* window virtual class declaration */
#ifndef FLX_WINDOW_H
#define FLX_WINDOW_H
#include <runtime/updateable.h>
namespace falx {
	void InitWindowSystem();
	void ShutdownWindowSystem();
	class IWindow : public IUpdateable {
	public:
		// false on failure
		virtual bool Create(const char* aTitle, int2 aWindowSize) = 0;
		// True if the window is to be closed
		virtual bool ShouldClose() = 0;
	};
	// Throws an assert if the window creation fails for debug builds
	// Else (for release builds) it will just NULL the pointer and return
	// -
	// 
	void CreateWindow(IWindow*& aWindow, const char* aTitle, int2 aWindowSize);
}
#endif