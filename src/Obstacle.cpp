#include "Obstacle.h"
#include <cmath>

Obstacle::Obstacle(float startX, float startY, int w, int h, ObstacleType obstacleType)
    : x(startX), y(startY), startY(startY), width(w), height(h), 
      active(true), type(obstacleType), timeAlive(0), velocityX(-3.0f), velocityY(0) {
    
    if (type == ObstacleType::MOVING_VERTICAL) {
        velocityY = 2.0f;
    }
}

void Obstacle::update() {
    if (!active) return;
    
    timeAlive += 0.05f;
    x += velocityX;
    
    switch(type) {
        case ObstacleType::STATIC_BARRIER:
            // Just scrolls with velocityX
            break;
            
        case ObstacleType::MOVING_VERTICAL:
            y += velocityY;
            // Bounce between bounds
            if (y <= 100 || y >= 600) {
                velocityY = -velocityY;
            }
            break;
            
        case ObstacleType::ROTATING:
            // Visual rotation (not implemented in rendering yet)
            break;
    }
    
    // Deactivate if off-screen
    if (x < -width) {
        active = false;
    }
}

void Obstacle::render(SDL_Renderer* renderer) {
    if (!active) return;
    
    // Different colors for different types
    switch(type) {
        case ObstacleType::STATIC_BARRIER:
            SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
            break;
        case ObstacleType::MOVING_VERTICAL:
            SDL_SetRenderDrawColor(renderer, 150, 80, 150, 255);
            break;
        case ObstacleType::ROTATING:
            SDL_SetRenderDrawColor(renderer, 180, 120, 60, 255);
            break;
    }
    
    SDL_Rect rect = {static_cast<int>(x), static_cast<int>(y), width, height};
    SDL_RenderFillRect(renderer, &rect);
    
    // Border for visibility
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

SDL_Rect Obstacle::getBounds() const {
    return {static_cast<int>(x), static_cast<int>(y), width, height};
}
