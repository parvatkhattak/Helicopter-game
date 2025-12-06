#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <SDL2/SDL.h>

class CollisionDetector {
public:
    // AABB collision detection
    static bool checkCollision(const SDL_Rect& a, const SDL_Rect& b);
    
    // Point-in-rect collision
    static bool pointInRect(int x, int y, const SDL_Rect& rect);
    
    // Circle collision (for more precise detection)
    static bool checkCircleCollision(int x1, int y1, int r1, int x2, int y2, int r2);
    
    // Get collision overlap
    static SDL_Rect getOverlap(const SDL_Rect& a, const SDL_Rect& b);
};

#endif
