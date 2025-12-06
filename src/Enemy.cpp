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
    
    // Enemy helicopter (red)
    SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
    SDL_Rect body = {static_cast<int>(x), static_cast<int>(y + 10), 30, 12};
    SDL_RenderFillRect(renderer, &body);
    
    // Cockpit
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_Rect cockpit = {static_cast<int>(x + 5), static_cast<int>(y + 5), 12, 15};
    SDL_RenderFillRect(renderer, &cockpit);
    
    // Tail
    SDL_SetRenderDrawColor(renderer, 150, 40, 40, 255);
    SDL_Rect tail = {static_cast<int>(x + 25), static_cast<int>(y + 12), 12, 8};
    SDL_RenderFillRect(renderer, &tail);
    
    // Rotor
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect rotor = {static_cast<int>(x + 8), static_cast<int>(y), 15, 2};
    SDL_RenderFillRect(renderer, &rotor);
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
