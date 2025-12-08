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
    
    // Apply impulse on thrust tap (instead of continuous thrust)
    // Note: thrustKey will be true for one frame per tap
    if (thrustKey) {
        velocityY = THRUST;  // Set velocity directly for instant response
    }
    
    // Always apply gravity
    velocityY += GRAVITY;
    
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
    
    int intX = static_cast<int>(x);
    int intY = static_cast<int>(y);
    
    // Shadow
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);
    SDL_Rect shadow = {intX + 2, intY + 32, 48, 8};
    SDL_RenderFillRect(renderer, &shadow);
    
    // Tail boom
    SDL_SetRenderDrawColor(renderer, 40, 120, 40, 255);
    SDL_Rect tailBoom = {intX - 10, intY + 12, 15, 10};
    SDL_RenderFillRect(renderer, &tailBoom);
    
    // Tail rotor
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect tailRotor = {intX - 12, intY + 12, 3, 10};
    SDL_RenderFillRect(renderer, &tailRotor);
    
    // Main body (darker base)
    SDL_SetRenderDrawColor(renderer, 40, 130, 40, 255);
    SDL_Rect bodyBase = {intX, intY + 12, 40, 15};
    SDL_RenderFillRect(renderer, &bodyBase);
    
    // Main body (lighter top for gradient effect)
    SDL_SetRenderDrawColor(renderer, 50, 160, 50, 255);
    SDL_Rect bodyTop = {intX, intY + 10, 40, 8};
    SDL_RenderFillRect(renderer, &bodyTop);
    
    // Cockpit window (glass)
    SDL_SetRenderDrawColor(renderer, 100, 200, 250, 180);
    SDL_Rect cockpitGlass = {intX + 30, intY + 7, 13, 12};
    SDL_RenderFillRect(renderer, &cockpitGlass);
    
    // Cockpit frame
    SDL_SetRenderDrawColor(renderer, 80, 180, 80, 255);
    SDL_Rect cockpit = {intX + 28, intY + 5, 17, 20};
    SDL_RenderDrawRect(renderer, &cockpit);
    
    // Cockpit highlight
    SDL_SetRenderDrawColor(renderer, 120, 220, 120, 255);
    SDL_Rect cockpitHighlight = {intX + 31, intY + 6, 10, 2};
    SDL_RenderFillRect(renderer, &cockpitHighlight);
    
    // Landing skid
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect skid1 = {intX + 5, intY + 28, 30, 2};
    SDL_RenderFillRect(renderer, &skid1);
    SDL_Rect skidSupport1 = {intX + 10, intY + 25, 2, 5};
    SDL_RenderFillRect(renderer, &skidSupport1);
    SDL_Rect skidSupport2 = {intX + 28, intY + 25, 2, 5};
    SDL_RenderFillRect(renderer, &skidSupport2);
    
    // Main rotor mast
    SDL_SetRenderDrawColor(renderer, 60, 60, 60, 255);
    SDL_Rect rotorMast = {intX + 18, intY, 4, 10};
    SDL_RenderFillRect(renderer, &rotorMast);
    
    // Main rotor blades (spinning effect)
    static int rotorFrame = 0;
    rotorFrame = (rotorFrame + 1) % 4;
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
    if (rotorFrame % 2 == 0) {
        SDL_Rect rotor1 = {intX + 5, intY + 2, 30, 3};
        SDL_RenderFillRect(renderer, &rotor1);
    } else {
        SDL_Rect rotor2 = {intX + 18, intY - 8, 3, 20};
        SDL_RenderFillRect(renderer, &rotor2);
    }
    
    // Rotor hub
    SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
    SDL_Rect rotorHub = {intX + 17, intY + 1, 6, 4};
    SDL_RenderFillRect(renderer, &rotorHub);
    
    // Engine exhaust (if thrusting)
    if (velocityY < 0) {
        SDL_SetRenderDrawColor(renderer, 255, 100, 0, 150);
        SDL_Rect exhaust = {intX + 8, intY + 26, 4, 2};
        SDL_RenderFillRect(renderer, &exhaust);
    }
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
