#include "Enemy.h"
#include <cmath>

Enemy::Enemy(float startX, float startY, EnemyType enemyType)
    : x(startX), y(startY), startY(startY), velocityX(-2.0f),
      width(40), height(30), health(30), active(true), type(enemyType), timeAlive(0) {
}

void Enemy::update(float playerX, float playerY) {
    if (!active) return;
    
    timeAlive += 0.1f;
    
    switch(type) {
        case EnemyType::STATIONARY:
            x += velocityX;
            break;
            
        case EnemyType::SINE_WAVE:
            x += velocityX;
            y = startY + std::sin(timeAlive) * 50.0f;
            break;
            
        case EnemyType::CHARGING:
            x += velocityX;
            // Move towards player vertically
            if (y < playerY - 10) {
                y += 1.5f;
            } else if (y > playerY + 10) {
                y -= 1.5f;
            }
            break;
    }
    
    // Deactivate if off-screen
    if (x < -width) {
        active = false;
    }
}

void Enemy::render(SDL_Renderer* renderer) {
    if (!active) return;
    
    int intX = static_cast<int>(x);
    int intY = static_cast<int>(y);
    
    // Shadow
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 60);
    SDL_Rect shadow = {intX + 2, intY + 28, 38, 6};
    SDL_RenderFillRect(renderer, &shadow);
    
    // Tail rotor
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect tailRotor = {intX + 36, intY + 12, 3, 8};
    SDL_RenderFillRect(renderer, &tailRotor);
    
    // Tail boom
    SDL_SetRenderDrawColor(renderer, 150, 40, 40, 255);
    SDL_Rect tailBoom = {intX + 25, intY + 12, 12, 8};
    SDL_RenderFillRect(renderer, &tailBoom);
    
    // Main body (darker base)
    SDL_SetRenderDrawColor(renderer, 180, 30, 30, 255);
    SDL_Rect bodyBase = {intX, intY + 12, 30, 12};
    SDL_RenderFillRect(renderer, &bodyBase);
    
    // Main body (lighter top for gradient)
    SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
    SDL_Rect bodyTop = {intX, intY + 10, 30, 6};
    SDL_RenderFillRect(renderer, &bodyTop);
    
    // Cockpit window (dark glass)
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 200);
    SDL_Rect cockpitGlass = {intX + 5, intY + 7, 10, 10};
    SDL_RenderFillRect(renderer, &cockpitGlass);
    
    // Cockpit frame
    SDL_SetRenderDrawColor(renderer, 200, 60, 60, 255);
    SDL_Rect cockpit = {intX + 3, intY + 5, 14, 15};
    SDL_RenderDrawRect(renderer, &cockpit);
    
    // Landing skid
    SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255);
    SDL_Rect skid = {intX + 4, intY + 24, 22, 2};
    SDL_RenderFillRect(renderer, &skid);
    SDL_Rect skidSupport = {intX + 8, intY + 22, 2, 3};
    SDL_RenderFillRect(renderer, &skidSupport);
    
    // Rotor mast
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect rotorMast = {intX + 13, intY + 1, 3, 9};
    SDL_RenderFillRect(renderer, &rotorMast);
    
    // Rotor blades (spinning)
    static int rotorFrame = 0;
    rotorFrame = (rotorFrame + 1) % 4;
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 180);
    if (rotorFrame % 2 == 0) {
        SDL_Rect rotor = {intX + 3, intY + 3, 24, 2};
        SDL_RenderFillRect(renderer, &rotor);
    } else {
        SDL_Rect rotor = {intX + 13, intY - 6, 2, 16};
        SDL_RenderFillRect(renderer, &rotor);
    }
    
    // Rotor hub
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect rotorHub = {intX + 12, intY + 2, 5, 3};
    SDL_RenderFillRect(renderer, &rotorHub);
    
    // Warning lights (blinking for charging type)
    if (type == EnemyType::CHARGING) {
        static int blinkFrame = 0;
        blinkFrame = (blinkFrame + 1) % 30;
        if (blinkFrame < 15) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_Rect light1 = {intX + 2, intY + 15, 2, 2};
            SDL_Rect light2 = {intX + 26, intY + 15, 2, 2};
            SDL_RenderFillRect(renderer, &light1);
            SDL_RenderFillRect(renderer, &light2);
        }
    }
}

void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        active = false;
    }
}

SDL_Rect Enemy::getBounds() const {
    return {static_cast<int>(x), static_cast<int>(y), width, height};
}
