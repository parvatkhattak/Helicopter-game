#ifndef HELICOPTER_H
#define HELICOPTER_H

#include <SDL2/SDL.h>
#include <vector>

class Bullet;

class Helicopter {
private:
    float x, y;                    // Position
    float velocityY;               // Vertical velocity
    float rotation;                // Rotation angle for tilt effect
    int width, height;             // Dimensions
    int health;                    // Health points
    bool isAlive;                  // Alive status
    
    // Physics constants
    static constexpr float GRAVITY = 0.5f;
    static constexpr float THRUST = -1.2f;
    static constexpr float MAX_FALL_SPEED = 10.0f;
    static constexpr float MAX_RISE_SPEED = -8.0f;
    
    // Shooting
    int shootCooldown;
    static constexpr int SHOOT_DELAY = 15; // frames
    
public:
    Helicopter(float startX, float startY);
    
    void update(bool thrustKey, bool shootKey, std::vector<Bullet*>& bullets);
    void render(SDL_Renderer* renderer);
    void takeDamage(int damage);
    void reset(float startX, float startY);
    
    // Getters
    SDL_Rect getBounds() const;
    int getHealth() const { return health; }
    bool getIsAlive() const { return isAlive; }
    float getX() const { return x; }
    float getY() const { return y; }
};

#endif
