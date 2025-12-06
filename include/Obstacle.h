#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <SDL2/SDL.h>

enum class ObstacleType {
    STATIC_BARRIER,
    MOVING_VERTICAL,
    ROTATING
};

class Obstacle {
private:
    float x, y;
    float startY;              // For vertical movement
    int width, height;
    bool active;
    ObstacleType type;
    float timeAlive;
    float velocityX;
    float velocityY;
    
public:
    Obstacle(float startX, float startY, int w, int h, ObstacleType obstacleType);
    
    void update();
    void render(SDL_Renderer* renderer);
    
    // Getters
    SDL_Rect getBounds() const;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
};

#endif
