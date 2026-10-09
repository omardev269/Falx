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
		FLX_LOG("SDL3 window successfully created");
		return true;
	}
	bool SDL3Window::ShouldClose()
	{
		return m_ShouldClose;
	}
	bool SDL3Window::GetKey(Key aKey)
	{
		SDL_Scancode m_Scancode = KeyToScancode(aKey);
		if (m_Scancode == SDL_SCANCODE_UNKNOWN) {
			return false;
		}
		const bit* m_Scancodes = SDL_GetKeyboardState(NULL);
		return m_Scancodes[m_Scancode];
	}

	bool SDL3Window::GetMouseButton(MouseButton aButton)
	{
		int m_Button = MouseButtonToSDL(aButton);
		if (m_Button == 0) {
			return false;
		}
		return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON_MASK(m_Button);
	}

	void SDL3Window::GetMouseDelta(float2& oDelta)
	{
		oDelta = m_MouseDelta;
	}

	void SDL3Window::GetMousePos(float2& oPos)
	{
		oPos = m_MousePos;
	}
	
	void* SDL3Window::GetWindowsIdentifier()
	{
		SDL_PropertiesID h_Props = SDL_GetWindowProperties(p_Window);
		return SDL_GetPointerProperty(h_Props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, NULL);
	}
	void SDL3Window::BeginFrame()
	{
		SDL_Event h_Event;
		m_MouseDelta.x = 0.0f;
		m_MouseDelta.y = 0.0f;
		while (SDL_PollEvent(&h_Event)) {
			if (h_Event.type == SDL_EVENT_QUIT) {
				m_ShouldClose = true;
			}
			if (h_Event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
				m_ShouldClose = true;
			}
			if (h_Event.type == SDL_EVENT_MOUSE_MOTION) {
				m_MouseDelta.x = h_Event.motion.xrel;
				m_MouseDelta.y = h_Event.motion.yrel;
				m_MousePos.x = h_Event.motion.x;
				m_MousePos.y = h_Event.motion.y;
			}
			else {
				SDL_GetMouseState(&m_MousePos.x, &m_MousePos.y);
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
