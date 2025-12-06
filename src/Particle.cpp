#include "Particle.h"
#include <cstdlib>

Particle::Particle(float startX, float startY, float velX, float velY, Uint8 red, Uint8 green, Uint8 blue)
    : x(startX), y(startY), velocityX(velX), velocityY(velY), 
      size(4), lifetime(0), maxLifetime(60), r(red), g(green), b(blue), active(true) {
}

void Particle::update() {
    x += velocityX;
    y += velocityY;
    velocityY += 0.2f; // Gravity
    
    lifetime++;
    if (lifetime >= maxLifetime) {
        active = false;
    }
}

void Particle::render(SDL_Renderer* renderer) {
    if (!active) return;
    
    // Fade out
    float alpha = 1.0f - (static_cast<float>(lifetime) / maxLifetime);
    SDL_SetRenderDrawColor(renderer, r, g, b, static_cast<Uint8>(255 * alpha));
    
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), size, size};
    SDL_RenderFillRect(renderer, &rect);
}
