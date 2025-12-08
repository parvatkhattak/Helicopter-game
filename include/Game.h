#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Helicopter.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "Particle.h"
#include "Terrain.h"

enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAME_OVER
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    GameState state;
    
    // Game entities
    Helicopter* player;
    Terrain* terrain;
    std::vector<Bullet*> bullets;
    std::vector<Enemy*> enemies;
    std::vector<Obstacle*> obstacles;
    std::vector<Particle*> particles;
    
    // Game stats
    int score;
    int highScore;
    float distanceTraveled;
    int enemiesKilled;
    
    // Spawn timers
    int enemySpawnTimer;
    int obstacleSpawnTimer;
    
    // Input flags
    bool thrustPressed;  // For tap-based thrust control
    
    // Fonts
    TTF_Font* fontLarge;
    TTF_Font* fontMedium;
    TTF_Font* fontSmall;
    
    // Screen dimensions
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 720;
    static constexpr int FPS = 60;
    
    // Helper methods
    void handleEvents();
    void update();
    void render();
    void spawnEnemy();
    void spawnObstacle();
    void createExplosion(float x, float y, int count);
    void cleanupEntities();
    void resetGame();
    void renderMenu();
    void renderHUD();
    void renderGameOver();
    void loadHighScore();
    void saveHighScore();
    void renderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color, bool centered = false);
    
public:
    Game();
    ~Game();
    
    bool init();
    void run();
    void cleanup();
};

#endif
