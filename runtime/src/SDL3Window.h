// BismIllahIRRahmaanIRRaheem
/* SDL3 window implementation declaration */
#ifndef FLX_SDL3WINDOW_H
#define FLX_SDL3WINDOW_H
#include <runtime/window.h>
#include <SDL3/SDL.h>
namespace falx {

	class SDL3Window : public IWindow {
		SDL_Window* p_Window;
		bool m_ShouldClose;
	public:
		bool Create(const char* aTitle, int2 aWindowSize) override;
		bool ShouldClose() override;
		void BeginFrame() override;
		void EndFrame() override;
		void Dismiss() override;
	};
}
#endif // !FLX_SDL3WINDOW_H
