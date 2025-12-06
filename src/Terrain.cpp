#include "Terrain.h"
#include "CollisionDetector.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Terrain::Terrain(int width, int height, int speed) 
    : scrollSpeed(speed), screenWidth(width), screenHeight(height) {
    
    srand(time(nullptr));
    seed = rand();
    
    // Initialize terrain points
    for (int i = 0; i < screenWidth + 100; i += 10) {
        TerrainPoint point;
        point.x = i;
        point.groundY = generateHeight(i, false);
        point.ceilingY = generateHeight(i, true);
        points.push_back(point);
    }
}

float Terrain::noise(float x) {
    // Simple noise function
    float value = std::sin(x * 0.01f) * 50.0f + std::sin(x * 0.02f) * 30.0f + std::sin(x * 0.005f) * 80.0f;
    return value;
}

int Terrain::generateHeight(int x, bool isCeiling) {
    float height = noise(x + seed);
    
    if (isCeiling) {
        return static_cast<int>(80 + height * 0.3f);
    } else {
        return static_cast<int>(screenHeight - 120 - height * 0.3f);
    }
}

void Terrain::update() {
    // Scroll terrain
    for (auto& point : points) {
        point.x -= scrollSpeed;
    }
    
    // Remove off-screen points and add new ones
    while (!points.empty() && points[0].x < -20) {
        points.erase(points.begin());
    }
    
    while (points.back().x < screenWidth + 100) {
        TerrainPoint newPoint;
        newPoint.x = points.back().x + 10;
        newPoint.groundY = generateHeight(newPoint.x, false);
        newPoint.ceilingY = generateHeight(newPoint.x, true);
        points.push_back(newPoint);
    }
}

void Terrain::render(SDL_Renderer* renderer) {
    // Render ceiling with gradient effect
    for (size_t i = 0; i < points.size() - 1; i++) {
        // Dark rocky ceiling
        SDL_SetRenderDrawColor(renderer, 60, 60, 80, 255);
        SDL_Rect ceilingRect = {
            points[i].x,
            0,
            points[i+1].x - points[i].x + 1,
            points[i].ceilingY
        };
        SDL_RenderFillRect(renderer, &ceilingRect);
        
        // Ceiling edge/border (darker)
        SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
        SDL_RenderDrawLine(renderer, points[i].x, points[i].ceilingY, 
                          points[i+1].x, points[i+1].ceilingY);
    }
    
    // Render ground with layers for depth
    for (size_t i = 0; i < points.size() - 1; i++) {
        int groundY = points[i].groundY;
        
        // Grass layer (top)
        SDL_SetRenderDrawColor(renderer, 80, 150, 60, 255);
        SDL_Rect grassRect = {
            points[i].x,
            groundY,
            points[i+1].x - points[i].x + 1,
            8
        };
        SDL_RenderFillRect(renderer, &grassRect);
        
        // Dirt layer (middle)
        SDL_SetRenderDrawColor(renderer, 100, 80, 50, 255);
        SDL_Rect dirtRect = {
            points[i].x,
            groundY + 8,
            points[i+1].x - points[i].x + 1,
            12
        };
        SDL_RenderFillRect(renderer, &dirtRect);
        
        // Rock layer (bottom)
        SDL_SetRenderDrawColor(renderer, 70, 60, 50, 255);
        SDL_Rect rockRect = {
            points[i].x,
            groundY + 20,
            points[i+1].x - points[i].x + 1,
            screenHeight - (groundY + 20)
        };
        SDL_RenderFillRect(renderer, &rockRect);
        
        // Ground edge highlight
        SDL_SetRenderDrawColor(renderer, 100, 180, 80, 255);
        SDL_RenderDrawLine(renderer, points[i].x, groundY, 
                          points[i+1].x, points[i+1].groundY);
    }
}

bool Terrain::checkCollision(const SDL_Rect& bounds) {
    // Check collision with terrain
    for (const auto& point : points) {
        if (point.x >= bounds.x && point.x <= bounds.x + bounds.w) {
            // Check ceiling collision
            if (bounds.y <= point.ceilingY) {
                return true;
            }
            // Check ground collision
            if (bounds.y + bounds.h >= point.groundY) {
                return true;
            }
        }
    }
    return false;
}

void Terrain::reset() {
    points.clear();
    seed = rand();
    
    for (int i = 0; i < screenWidth + 100; i += 10) {
        TerrainPoint point;
        point.x = i;
        point.groundY = generateHeight(i, false);
        point.ceilingY = generateHeight(i, true);
        points.push_back(point);
    }
}
