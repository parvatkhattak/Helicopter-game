#include "Game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game game;
    
    if (!game.init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        return 1;
    }
    
    std::cout << "=== Helicopter Game ===" << std::endl;
    std::cout << "Controls:" << std::endl;
    std::cout << "  SPACE/UP ARROW - Thrust (hold to rise, release to fall)" << std::endl;
    std::cout << "  SPACE/X - Shoot" << std::endl;
    std::cout << "  P/ESC - Pause" << std::endl;
    std::cout << "  ESC - Quit (from menu)" << std::endl;
    std::cout << std::endl;
    std::cout << "Objective: Avoid obstacles and terrain, shoot enemies!" << std::endl;
    std::cout << "Starting game..." << std::endl;
    
    game.run();
    game.cleanup();
    
    return 0;
}
