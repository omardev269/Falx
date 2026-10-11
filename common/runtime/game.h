// BismIllahIRRahmaanIRRaheem
/* game class declarations */

#ifndef GAME_FLX_H
#define GAME_FLX_H  
#include <runtime/enginedef.h>
// Game Class - to be implemented by the game itself
class Game {
    // Runs after all subsystems (ie.. windowing, graphics) start
    void Start();
    // Runs per frame
    // - RETURNS: bit - false if engine is to stop running - true if engine is to continue running
    bit PerFrame();
    // Runs before any subsystem ends
    void Quit();
};
#endif // !GAME_FLX_H
