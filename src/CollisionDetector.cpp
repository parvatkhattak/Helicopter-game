#include "CollisionDetector.h"
#include <cmath>
#include <algorithm>

bool CollisionDetector::checkCollision(const SDL_Rect& a, const SDL_Rect& b) {
    // AABB collision detection
    int leftA = a.x;
    int rightA = a.x + a.w;
    int topA = a.y;
    int bottomA = a.y + a.h;
    
    int leftB = b.x;
    int rightB = b.x + b.w;
    int topB = b.y;
    int bottomB = b.y + b.h;
    
    // Check if boxes are separated
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;
    
    return true;
}

bool CollisionDetector::pointInRect(int x, int y, const SDL_Rect& rect) {
    return (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h);
}

bool CollisionDetector::checkCircleCollision(int x1, int y1, int r1, int x2, int y2, int r2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int distance = std::sqrt(dx * dx + dy * dy);
    return distance < (r1 + r2);
}

SDL_Rect CollisionDetector::getOverlap(const SDL_Rect& a, const SDL_Rect& b) {
    SDL_Rect overlap;
    overlap.x = std::max(a.x, b.x);
    overlap.y = std::max(a.y, b.y);
    overlap.w = std::min(a.x + a.w, b.x + b.w) - overlap.x;
    overlap.h = std::min(a.y + a.h, b.y + b.h) - overlap.y;
    return overlap;
}
