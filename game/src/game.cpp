// BismIllahIRRahmaanIRRaheem
/* sample game class implementation */

#include <runtime/game.h>
#include <runtime/exposition.h>

void Game::Start() {
	
}

bit Game::PerFrame() {
	return !falx::g_iWindow->ShouldClose();
}

void Game::Quit() {
	
}