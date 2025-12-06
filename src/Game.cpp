#include "Game.h"
#include "CollisionDetector.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game() 
    : window(nullptr), renderer(nullptr), running(false), 
      state(GameState::MENU), player(nullptr), terrain(nullptr),
      score(0), highScore(0), distanceTraveled(0), enemiesKilled(0),
      enemySpawnTimer(0), obstacleSpawnTimer(0) {
    
    srand(time(nullptr));
}

Game::~Game() {
    cleanup();
}

bool Game::init() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create window
    window = SDL_CreateWindow("Helicopter Game", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, 
                              SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Initialize game objects
    player = new Helicopter(100, SCREEN_HEIGHT / 2);
    terrain = new Terrain(SCREEN_WIDTH, SCREEN_HEIGHT, 3);
    
    loadHighScore();
    
    running = true;
    return true;
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        if (event.type == SDL_KEYDOWN) {
            switch (state) {
                case GameState::MENU:
                    if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
                        state = GameState::PLAYING;
                        resetGame();
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        running = false;
                    }
                    break;
                    
                case GameState::PLAYING:
                    if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_p) {
                        state = GameState::PAUSED;
                    }
                    break;
                    
                case GameState::PAUSED:
                    if (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_p) {
                        state = GameState::PLAYING;
                    }
                    break;
                    
                case GameState::GAME_OVER:
                    if (event.key.keysym.sym == SDLK_SPACE || event.key.keysym.sym == SDLK_RETURN) {
                        state = GameState::PLAYING;
                        resetGame();
                    }
                    if (event.key.keysym.sym == SDLK_ESCAPE) {
                        state = GameState::MENU;
                    }
                    break;
            }
        }
    }
}

void Game::update() {
    if (state != GameState::PLAYING) return;
    
    // Get keyboard state
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    bool thrust = keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_UP];
    bool shoot = keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_X];
    
    // Update player
    player->update(thrust, shoot, bullets);
    
    // Update terrain
    terrain->update();
    
    // Update bullets
    for (auto* bullet : bullets) {
        bullet->update();
    }
    
    // Update enemies
    for (auto* enemy : enemies) {
        enemy->update(player->getX(), player->getY());
    }
    
    // Update obstacles
    for (auto* obstacle : obstacles) {
        obstacle->update();
    }
    
    // Update particles
    for (auto* particle : particles) {
        particle->update();
    }
    
    // Spawn enemies
    enemySpawnTimer++;
    if (enemySpawnTimer > 120) {
        spawnEnemy();
        enemySpawnTimer = 0;
    }
    
    // Spawn obstacles
    obstacleSpawnTimer++;
    if (obstacleSpawnTimer > 90) {
        spawnObstacle();
        obstacleSpawnTimer = 0;
    }
    
    // Collision detection - Bullets vs Enemies
    for (auto* bullet : bullets) {
        if (!bullet->isActive()) continue;
        
        for (auto* enemy : enemies) {
            if (!enemy->isActive()) continue;
            
            if (CollisionDetector::checkCollision(bullet->getBounds(), enemy->getBounds())) {
                bullet->deactivate();
                enemy->takeDamage(15);
                
                if (!enemy->isActive()) {
                    createExplosion(enemy->getBounds().x + 20, enemy->getBounds().y + 15, 20);
                    enemiesKilled++;
                    score += 100;
                }
                break;
            }
        }
    }
    
    // Collision detection - Player vs Enemies
    for (auto* enemy : enemies) {
        if (!enemy->isActive()) continue;
        
        if (CollisionDetector::checkCollision(player->getBounds(), enemy->getBounds())) {
            player->takeDamage(50);
            enemy->deactivate();
            createExplosion(enemy->getBounds().x + 20, enemy->getBounds().y + 15, 15);
            
            if (!player->getIsAlive()) {
                state = GameState::GAME_OVER;
                if (score > highScore) {
                    highScore = score;
                    saveHighScore();
                }
            }
        }
    }
    
    // Collision detection - Player vs Obstacles
    for (auto* obstacle : obstacles) {
        if (!obstacle->isActive()) continue;
        
        if (CollisionDetector::checkCollision(player->getBounds(), obstacle->getBounds())) {
            player->takeDamage(100);
            createExplosion(player->getX() + 25, player->getY() + 15, 25);
            state = GameState::GAME_OVER;
            
            if (score > highScore) {
                highScore = score;
                saveHighScore();
            }
        }
    }
    
    // Collision detection - Player vs Terrain
    if (terrain->checkCollision(player->getBounds())) {
        player->takeDamage(100);
        createExplosion(player->getX() + 25, player->getY() + 15, 25);
        state = GameState::GAME_OVER;
        
        if (score > highScore) {
            highScore = score;
            saveHighScore();
        }
    }
    
    // Update score
    distanceTraveled += 0.1f;
    score = static_cast<int>(distanceTraveled) + enemiesKilled * 100;
    
    // Cleanup inactive entities
    cleanupEntities();
}

void Game::render() {
    // Clear screen with sky blue
    SDL_SetRenderDrawColor(renderer, 135, 206, 235, 255);
    SDL_RenderClear(renderer);
    
    switch (state) {
        case GameState::MENU:
            renderMenu();
            break;
            
        case GameState::PLAYING:
        case GameState::PAUSED:
        case GameState::GAME_OVER:
            // Render terrain
            terrain->render(renderer);
            
            // Render obstacles
            for (auto* obstacle : obstacles) {
                obstacle->render(renderer);
            }
            
            // Render enemies
            for (auto* enemy : enemies) {
                enemy->render(renderer);
            }
            
            // Render bullets
            for (auto* bullet : bullets) {
                bullet->render(renderer);
            }
            
            // Render particles
            for (auto* particle : particles) {
                particle->render(renderer);
            }
            
            // Render player
            player->render(renderer);
            
            // Render HUD
            renderHUD();
            
            if (state == GameState::PAUSED) {
                // Pause overlay
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);
                SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
                SDL_RenderFillRect(renderer, &overlay);
                
                // Pause text would go here (simplified without SDL_ttf)
            }
            
            if (state == GameState::GAME_OVER) {
                renderGameOver();
            }
            break;
    }
    
    SDL_RenderPresent(renderer);
}

void Game::renderMenu() {
    // Title background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect titleBg = {SCREEN_WIDTH/2 - 300, 150, 600, 400};
    SDL_RenderFillRect(renderer, &titleBg);
    
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_Rect titleBorder = {SCREEN_WIDTH/2 - 305, 145, 610, 410};
    SDL_RenderDrawRect(renderer, &titleBorder);
    
    // Simple text replacement (without SDL_ttf, we'll use basic shapes)
    // In a real implementation, you'd use SDL_ttf here
    
    // "Press SPACE to Start" indicator
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect startIndicator = {SCREEN_WIDTH/2 - 100, 400, 200, 40};
    SDL_RenderFillRect(renderer, &startIndicator);
}

void Game::renderHUD() {
    // Health bar
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect healthBg = {10, 10, 200, 20};
    SDL_RenderFillRect(renderer, &healthBg);
    
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_Rect health = {10, 10, player->getHealth() * 2, 20};
    SDL_RenderFillRect(renderer, &health);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &healthBg);
    
    // Score indicator (simplified - just colored rectangles)
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect scoreBox = {SCREEN_WIDTH - 150, 10, 140, 30};
    SDL_RenderFillRect(renderer, &scoreBox);
    
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &scoreBox);
}

void Game::renderGameOver() {
    // Game over overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Game over box
    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);
    SDL_Rect gameOverBox = {SCREEN_WIDTH/2 - 250, 200, 500, 300};
    SDL_RenderFillRect(renderer, &gameOverBox);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &gameOverBox);
    
    // Restart indicator
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_Rect restartBox = {SCREEN_WIDTH/2 - 150, 400, 300, 50};
    SDL_RenderFillRect(renderer, &restartBox);
}

void Game::spawnEnemy() {
    int randY = 150 + rand() % 400;
    EnemyType type = static_cast<EnemyType>(rand() % 3);
    enemies.push_back(new Enemy(SCREEN_WIDTH, randY, type));
}

void Game::spawnObstacle() {
    int randY = 100 + rand() % 500;
    int randH = 40 + rand() % 100;
    ObstacleType type = static_cast<ObstacleType>(rand() % 3);
    obstacles.push_back(new Obstacle(SCREEN_WIDTH, randY, 30, randH, type));
}

void Game::createExplosion(float x, float y, int count) {
    for (int i = 0; i < count; i++) {
        float velX = -3.0f + static_cast<float>(rand() % 60) / 10.0f;
        float velY = -5.0f + static_cast<float>(rand() % 100) / 10.0f;
        Uint8 r = 200 + rand() % 56;
        Uint8 g = 100 + rand() % 100;
        particles.push_back(new Particle(x, y, velX, velY, r, g, 0));
    }
}

void Game::cleanupEntities() {
    // Remove inactive bullets
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(),
            [](Bullet* b) { 
                if (!b->isActive()) {
                    delete b;
                    return true;
                }
                return false;
            }),
        bullets.end()
    );
    
    // Remove inactive enemies
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [](Enemy* e) { 
                if (!e->isActive()) {
                    delete e;
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
    
    // Remove inactive obstacles
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [](Obstacle* o) { 
                if (!o->isActive()) {
                    delete o;
                    return true;
                }
                return false;
            }),
        obstacles.end()
    );
    
    // Remove inactive particles
    particles.erase(
        std::remove_if(particles.begin(), particles.end(),
            [](Particle* p) { 
                if (!p->isActive()) {
                    delete p;
                    return true;
                }
                return false;
            }),
        particles.end()
    );
}

void Game::resetGame() {
    // Clear all entities
    for (auto* b : bullets) delete b;
    for (auto* e : enemies) delete e;
    for (auto* o : obstacles) delete o;
    for (auto* p : particles) delete p;
    
    bullets.clear();
    enemies.clear();
    obstacles.clear();
    particles.clear();
    
    // Reset player and terrain
    player->reset(100, SCREEN_HEIGHT / 2);
    terrain->reset();
    
    // Reset stats
    score = 0;
    distanceTraveled = 0;
    enemiesKilled = 0;
    enemySpawnTimer = 0;
    obstacleSpawnTimer = 0;
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    }
}

void Game::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    }
}

void Game::run() {
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;
    
    while (running) {
        frameStart = SDL_GetTicks();
        
        handleEvents();
        update();
        render();
        
        frameTime = SDL_GetTicks() - frameStart;
        
        if (frameDelay > frameTime) {
            SDL_Delay(frameDelay - frameTime);
        }
    }
}

void Game::cleanup() {
    // Delete all entities with nullptr checks
    if (player) {
        delete player;
        player = nullptr;
    }
    if (terrain) {
        delete terrain;
        terrain = nullptr;
    }
    
    for (auto* b : bullets) delete b;
    for (auto* e : enemies) delete e;
    for (auto* o : obstacles) delete o;
    for (auto* p : particles) delete p;
    
    bullets.clear();
    enemies.clear();
    obstacles.clear();
    particles.clear();
    
    // Destroy SDL objects
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    SDL_Quit();
}
