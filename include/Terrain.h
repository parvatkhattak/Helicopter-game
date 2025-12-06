#ifndef TERRAIN_H
#define TERRAIN_H

#include <SDL2/SDL.h>
#include <vector>

class Terrain {
private:
    struct TerrainPoint {
        int x;
        int groundY;
        int ceilingY;
    };
    
    std::vector<TerrainPoint> points;
    int scrollSpeed;
    int screenWidth, screenHeight;
    int seed;
    
    int generateHeight(int x, bool isCeiling);
    float noise(float x);
    
public:
    Terrain(int width, int height, int speed);
    
    void update();
    void render(SDL_Renderer* renderer);
    void reset();
    
    // Collision check
    bool checkCollision(const SDL_Rect& bounds);
};

#endif
