#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>

enum class EnemyType {
    STATIONARY,
    SINE_WAVE,
    CHARGING
};

class Enemy {
private:
    float x, y;
    float startY;              // For sine wave pattern
    float velocityX;
    int width, height;
    int health;
    bool active;
    EnemyType type;
    float timeAlive;           // For movement patterns
    
public:
    Enemy(float startX, float startY, EnemyType enemyType);
    
    void update(float playerX, float playerY);
    void render(SDL_Renderer* renderer);
    void takeDamage(int damage);
    
    // Getters
    SDL_Rect getBounds() const;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
    int getHealth() const { return health; }
};

#endif
