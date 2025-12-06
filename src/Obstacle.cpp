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
    
    int intX = static_cast<int>(x);
    int intY = static_cast<int>(y);
    
    // Shadow
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 40);
    SDL_Rect shadow = {intX + 2, intY + 2, width, height};
    SDL_RenderFillRect(renderer, &shadow);
    
    // Different colors and effects for different types
    switch(type) {
        case ObstacleType::STATIC_BARRIER: {
            // 3D stone block effect
            SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255);
            SDL_Rect baseRect = {intX, intY, width, height};
            SDL_RenderFillRect(renderer, &baseRect);
            
            // Highlight (top-left)
            SDL_SetRenderDrawColor(renderer, 160, 160, 160, 255);
            SDL_Rect highlight = {intX, intY, width - 2, 3};
            SDL_RenderFillRect(renderer, &highlight);
            SDL_Rect highlight2 = {intX, intY, 3, height - 2};
            SDL_RenderFillRect(renderer, &highlight2);
            
            // Shadow edge (bottom-right)
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_Rect shadowEdge = {intX + 2, intY + height - 3, width - 2, 3};
            SDL_RenderFillRect(renderer, &shadowEdge);
            SDL_Rect shadowEdge2 = {intX + width - 3, intY + 2, 3, height - 2};
            SDL_RenderFillRect(renderer, &shadowEdge2);
            break;
        }
            
        case ObstacleType::MOVING_VERTICAL: {
            // Purple energy barrier with glow
            SDL_SetRenderDrawColor(renderer, 180, 100, 180, 255);
            SDL_Rect energyCore = {intX, intY, width, height};
            SDL_RenderFillRect(renderer, &energyCore);
            
            // Glow effect
            SDL_SetRenderDrawColor(renderer, 220, 140, 220, 150);
            SDL_Rect glow = {intX - 1, intY - 1, width + 2, height + 2};
            SDL_RenderDrawRect(renderer, &glow);
            
            // Energy lines
            SDL_SetRenderDrawColor(renderer, 255, 200, 255, 255);
            for (int i = 0; i < height; i += 8) {
                SDL_RenderDrawLine(renderer, intX, intY + i, intX + width, intY + i);
            }
            break;
        }
            
        case ObstacleType::ROTATING: {
            // Orange/brown rotating hazard
            SDL_SetRenderDrawColor(renderer, 200, 130, 70, 255);
            SDL_Rect rotatingBase = {intX, intY, width, height};
            SDL_RenderFillRect(renderer, &rotatingBase);
            
            // Diagonal stripes for hazard warning
            SDL_SetRenderDrawColor(renderer, 255, 200, 100, 255);
            for (int i = 0; i < width + height; i += 10) {
                SDL_RenderDrawLine(renderer, intX + i, intY, intX, intY + i);
            }
            
            // Border
            SDL_SetRenderDrawColor(renderer, 150, 80, 40, 255);
            SDL_Rect border = {intX, intY, width, height};
            SDL_RenderDrawRect(renderer, &border);
            break;
        }
    }
    
    // Main border for all types
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect mainBorder = {intX, intY, width, height};
    SDL_RenderDrawRect(renderer, &mainBorder);
}

SDL_Rect Obstacle::getBounds() const {
    return {static_cast<int>(x), static_cast<int>(y), width, height};
}
