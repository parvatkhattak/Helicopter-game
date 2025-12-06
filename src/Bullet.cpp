#include "Bullet.h"

Bullet::Bullet(float startX, float startY) 
    : x(startX), y(startY), velocityX(12.0f), width(10), height(4), active(true) {
}

void Bullet::update() {
    x += velocityX;
    
    // Deactivate if off-screen
    if (x > 1400) {
        active = false;
    }
}

void Bullet::render(SDL_Renderer* renderer) {
    if (!active) return;
    
    // Yellow bullet
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), width, height};
    SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Bullet::getBounds() const {
    return {static_cast<int>(x), static_cast<int>(y), width, height};
}
