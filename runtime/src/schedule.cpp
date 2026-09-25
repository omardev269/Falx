// BismIllahIRRahmaanIRRaheem
/* runtime stuff manager */

#include <runtime/enginedef.h>
#include <timeint.h>
#include <runtime/rtdebug.h>
#include <runtime/time.h>
#include <SDL3/SDL.h>
#include <format>

namespace falx {
	SDL_Window* g_pWindow;
}

void FalxStart() {
	using namespace falx;
	InitializeTimeInt();
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	g_pWindow = SDL_CreateWindow("AlhamdullIllah!", 800, 600, SDL_WINDOW_OPENGL);
	FLX_SMART_CHECK(g_pWindow, "Failed to create SDL window");
}
bool FalxUpdate() {
	using namespace falx;
	{
		SDL_Event h_Event;
		while (SDL_PollEvent(&h_Event)) {
			if (h_Event.type == SDL_EVENT_QUIT) {
				return false;
			}
		}
	}
	// false if the engine should stop
	return true;
}
void FalxStop() {
	using namespace falx;
	SDL_DestroyWindow(g_pWindow);
	SDL_Quit();
}