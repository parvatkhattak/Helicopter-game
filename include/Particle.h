#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL2/SDL.h>

class Particle {
private:
    float x, y;
    float velocityX, velocityY;
    int size;
    int lifetime;
    int maxLifetime;
    Uint8 r, g, b;
    bool active;
    
public:
    Particle(float startX, float startY, float velX, float velY, Uint8 red, Uint8 green, Uint8 blue);
    
    void update();
    void render(SDL_Renderer* renderer);
    
    bool isActive() const { return active; }
};

#endif
