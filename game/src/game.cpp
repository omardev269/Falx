// BismIllahIRRahmaanIRRaheem
/* sample game class implementation */

#include <runtime/game.h>
#include <runtime/exposition.h>
#include <graphics/loader.h>
#include <graphics/scene.h>
#include <graphics/freefly.h>
#include <imgui.h>
#include <graphics/graphicsdef.h>
#include <gb.h>
#include <gb_math.h>
#include <vector>

void Game::Start() {
	using namespace falx;
}

bit Game::PerFrame() {
	using namespace falx;
	bit m_Continue = !g_iWindow->ShouldClose();
	return m_Continue;
}

void Game::Quit() {
	using namespace falx;
}
