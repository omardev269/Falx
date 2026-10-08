// BismIllahIRRahmaanIRRaheem
/* SDL3 window implementation */
#include <SDL3Window.h>
#include <SDL3/SDL.h>
#include <runtime/rtdebug.h>
namespace falx {
	// Converts a falx::Key to its SDL_Scancode (SDL_SCANCODE_UNKNOWN if unmapped)
	static SDL_Scancode KeyToScancode(Key aKey)
	{
		if (aKey >= Key::A && aKey <= Key::Z)
			return (SDL_Scancode)(SDL_SCANCODE_A + ((int)aKey - (int)Key::A));
		if (aKey >= Key::Num1 && aKey <= Key::Num9)
			return (SDL_Scancode)(SDL_SCANCODE_1 + ((int)aKey - (int)Key::Num1));
		if (aKey >= Key::F1 && aKey <= Key::F12)
			return (SDL_Scancode)(SDL_SCANCODE_F1 + ((int)aKey - (int)Key::F1));
		switch (aKey) {
		case Key::Num0: return SDL_SCANCODE_0;
		case Key::Up: return SDL_SCANCODE_UP;
		case Key::Down: return SDL_SCANCODE_DOWN;
		case Key::Left: return SDL_SCANCODE_LEFT;
		case Key::Right: return SDL_SCANCODE_RIGHT;
		case Key::Space: return SDL_SCANCODE_SPACE;
		case Key::Enter: return SDL_SCANCODE_RETURN;
		case Key::Escape: return SDL_SCANCODE_ESCAPE;
		case Key::Tab: return SDL_SCANCODE_TAB;
		case Key::Backspace: return SDL_SCANCODE_BACKSPACE;
		case Key::LeftShift: return SDL_SCANCODE_LSHIFT;
		case Key::RightShift: return SDL_SCANCODE_RSHIFT;
		case Key::LeftControl: return SDL_SCANCODE_LCTRL;
		case Key::RightControl: return SDL_SCANCODE_RCTRL;
		case Key::LeftAlt: return SDL_SCANCODE_LALT;
		case Key::RightAlt: return SDL_SCANCODE_RALT;
		default: return SDL_SCANCODE_UNKNOWN;
		}
	}
	// Converts a falx::MouseButton to its SDL button index (0 if unmapped)
	static int MouseButtonToSDL(MouseButton aButton)
	{
		switch (aButton) {
		case MouseButton::Left: return SDL_BUTTON_LEFT;
		case MouseButton::Middle: return SDL_BUTTON_MIDDLE;
		case MouseButton::Right: return SDL_BUTTON_RIGHT;
		default: return 0;
		}
	}
	void InitWindowSystem()
	{
		FLX_SMART_CHECK(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO), "SDL3 failed to initialize");
	}
	void ShutdownWindowSystem()
	{
		SDL_Quit();
	}
	bool SDL3Window::Create(const char* aTitle, int2 aWindowSize)
	{
		p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, SDL_WINDOW_OPENGL);
		if (!p_Window) {
			FLX_LOG("OpenGL is disabled for this window due to compatibility issues\n");
			p_Window = SDL_CreateWindow(aTitle, aWindowSize.x, aWindowSize.y, 0);
			if (!p_Window) {
				FLX_LOG("Window creation failed - ");
				FLX_LOG(SDL_GetError());
				FLX_LOG("\n");
				return false;
			}
		}
		return true;
	}
	bool SDL3Window::ShouldClose()
	{
		return m_ShouldClose;
	}
	void* SDL3Window::GetWindowsIdentifier()
	{

		SDL_PropertiesID h_Props = SDL_GetWindowProperties(p_Window);
		return SDL_GetPointerProperty(h_Props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
	}
	void SDL3Window::BeginFrame()
	{
		SDL_Event h_Event;
		while (SDL_PollEvent(&h_Event)) {
			if (h_Event.type == SDL_EVENT_QUIT) {
				m_ShouldClose = true;
			}
		}
	}
	void SDL3Window::EndFrame()
	{
	}
	void SDL3Window::Dismiss()
	{
		SDL_DestroyWindow(p_Window);
	}
}
