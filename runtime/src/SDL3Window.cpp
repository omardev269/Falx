// BismIllahIRRahmaanIRRaheem
/* SDL3 window implementation */
#include <SDL3Window.h>
#include <SDL3/SDL.h>
#include <runtime/rtdebug.h>
namespace falx {
	void InitWindowSystem()
	{
		SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
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
