#include "Helicopter.h"
#include "Bullet.h"
#include <cmath>
#include <algorithm>

Helicopter::Helicopter(float startX, float startY) 
    : x(startX), y(startY), velocityY(0), rotation(0),
      width(50), height(30), health(100), isAlive(true), shootCooldown(0) {
}

void Helicopter::update(bool thrustKey, bool shootKey, std::vector<Bullet*>& bullets) {
    if (!isAlive) return;
    
    // Apply thrust or gravity
    if (thrustKey) {
        velocityY += THRUST;
    } else {
        velocityY += GRAVITY;
    }
    
    // Clamp velocity
    velocityY = std::max(MAX_RISE_SPEED, std::min(MAX_FALL_SPEED, velocityY));
    
    // Update position
    y += velocityY;
    
    // Rotation based on velocity (tilt)
    rotation = velocityY * 3.0f;
    rotation = std::max(-30.0f, std::min(30.0f, rotation));
    
    // Shooting
    if (shootCooldown > 0) {
        shootCooldown--;
    }
    
    if (shootKey && shootCooldown == 0) {
        bullets.push_back(new Bullet(x + width, y + height / 2));
        shootCooldown = SHOOT_DELAY;
    }
    
    // Keep on screen (vertical bounds)
    if (y < 0) {
        y = 0;
        velocityY = 0;
    }
    if (y > 720 - height) {
        y = 720 - height;
        velocityY = 0;
    }
}

void Helicopter::render(SDL_Renderer* renderer) {
    if (!isAlive) return;
    
    // Helicopter body (simplified)
    SDL_SetRenderDrawColor(renderer, 50, 150, 50, 255);
    SDL_Rect body = {static_cast<int>(x), static_cast<int>(y + 10), 40, 15};
    SDL_RenderFillRect(renderer, &body);
    
    // Cockpit
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_Rect cockpit = {static_cast<int>(x + 30), static_cast<int>(y + 5), 15, 20};
    SDL_RenderFillRect(renderer, &cockpit);
    
    // Tail
    SDL_SetRenderDrawColor(renderer, 40, 120, 40, 255);
    SDL_Rect tail = {static_cast<int>(x - 10), static_cast<int>(y + 12), 15, 10};
    SDL_RenderFillRect(renderer, &tail);
    
    // Rotor (animated)
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect rotor = {static_cast<int>(x + 10), static_cast<int>(y), 20, 2};
    SDL_RenderFillRect(renderer, &rotor);
}

void Helicopter::takeDamage(int damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        isAlive = false;
    }
}

void Helicopter::reset(float startX, float startY) {
    x = startX;
    y = startY;
    velocityY = 0;
    rotation = 0;
    health = 100;
    isAlive = true;
    shootCooldown = 0;
}

SDL_Rect Helicopter::getBounds() const {
    return {static_cast<int>(x), static_cast<int>(y), width, height};
}
