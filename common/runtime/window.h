// BismIllahIRRahmaanIRRaheem
/* window virtual class declaration */
#ifndef FLX_WINDOW_H
#define FLX_WINDOW_H
#include <runtime/updateable.h>
namespace falx {
	void InitWindowSystem();
	void ShutdownWindowSystem();
	enum class Key : uint8 {
		Unknown = 0,
		A, B, C, D, E, F, G, H, I, J, K, L, M,
		N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
		Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
		F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
		Up, Down, Left, Right,
		Space, Enter, Escape, Tab, Backspace,
		LeftShift, RightShift, LeftControl, RightControl, LeftAlt, RightAlt,
		Count
	};
	enum class MouseButton : uint8 {
		Left = 0,
		Middle,
		Right,
		Count
	};
	class IWindow : public IUpdateable {
	public:
		// false on failure
		virtual bool Create(const char* aTitle, int2 aWindowSize) = 0;
		virtual void* GetWindowsIdentifier() = 0;

		// True if the window is to be closed
		virtual bool ShouldClose() = 0;
		// True if the key is currently held down
		virtual bool GetKey(Key aKey) = 0;
		// True if the mouse button is currently held down
		virtual bool GetMouseButton(MouseButton aButton) = 0;
		// Mouse movement (in pixels) since the last frame
		virtual void GetMouseDelta(float2& oDelta) = 0;
		// Mouse position (in pixels) relative to the window
		virtual void GetMousePos(float2& oPos) = 0;
	};
	// Throws an assert if the window creation fails for debug builds
	// Else (for release builds) it will just NULL the pointer and return
	// -
	// 
	void CreateWindowNW(IWindow*& aWindow, const char* aTitle, int2 aWindowSize);
}
#endif // !FLX_WINDOW_H