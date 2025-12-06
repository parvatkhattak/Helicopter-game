#ifndef BULLET_H
#define BULLET_H

#include <SDL2/SDL.h>

class Bullet {
private:
    float x, y;
    float velocityX;
    int width, height;
    bool active;
    
public:
    Bullet(float startX, float startY);
    
    void update();
    void render(SDL_Renderer* renderer);
    
    // Getters
    SDL_Rect getBounds() const;
    bool isActive() const { return active; }
    void deactivate() { active = false; }
};

#endif
