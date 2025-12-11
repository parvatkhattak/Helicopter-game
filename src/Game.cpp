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
      enemySpawnTimer(0), obstacleSpawnTimer(0), thrustPressed(false),
      fontLarge(nullptr), fontMedium(nullptr), fontSmall(nullptr) {
    
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
    
    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
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
    
    // Load fonts - try multiple common font paths
    const char* fontPaths[] = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/TTF/DejaVuSans-Bold.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Bold.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        "C:\\Windows\\Fonts\\arial.ttf"
    };
    
    bool fontLoaded = false;
    for (const char* fontPath : fontPaths) {
        fontLarge = TTF_OpenFont(fontPath, 72);
        fontMedium = TTF_OpenFont(fontPath, 48);
        fontSmall = TTF_OpenFont(fontPath, 24);
        
        if (fontLarge && fontMedium && fontSmall) {
            fontLoaded = true;
            std::cout << "Fonts loaded from: " << fontPath << std::endl;
            break;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Warning: Could not load fonts! Using fallback rendering." << std::endl;
        // Game can still run without fonts, just won't show text
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
    
    // Get keyboard state for continuous control
    const Uint8* keyState = SDL_GetKeyboardState(nullptr);
    bool thrust = keyState[SDL_SCANCODE_UP];  // Hold UP arrow for thrust
    bool shoot = keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_X];  // SPACE or X for shooting
    
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
    // Render gradient sky (light blue at top, lighter near horizon)
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        // Gradient from deep sky blue (top) to light horizon (bottom)
        float ratio = static_cast<float>(y) / SCREEN_HEIGHT;
        int r = static_cast<int>(135 + ratio * 100);  // 135 -> 235
        int g = static_cast<int>(206 + ratio * 40);   // 206 -> 246
        int b = static_cast<int>(235 + ratio * 20);   // 235 -> 255
        
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
    
    // Render parallax clouds (only during gameplay)
    if (state == GameState::PLAYING || state == GameState::PAUSED || state == GameState::GAME_OVER) {
        static int cloudOffset1 = 0;
        static int cloudOffset2 = 0;
        
        if (state == GameState::PLAYING) {
            cloudOffset1 = (cloudOffset1 + 1) % (SCREEN_WIDTH + 200);
            cloudOffset2 = (cloudOffset2 + 1) % (SCREEN_WIDTH + 300);
        }
        
        // Distant clouds (slower, lighter)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
        for (int i = 0; i < 4; i++) {
            int x = (i * 400 - cloudOffset2 / 2) % (SCREEN_WIDTH + 200) - 100;
            int y = 80 + i * 30;
            
            // Cloud shape (multiple ellipses)
            for (int j = 0; j < 3; j++) {
                SDL_Rect cloud = {x + j * 40, y, 80, 40};
                SDL_RenderFillRect(renderer, &cloud);
            }
        }
        
        // Closer clouds (faster, more opaque)
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 150);
        for (int i = 0; i < 3; i++) {
            int x = (i * 450 - cloudOffset1) % (SCREEN_WIDTH + 150) - 75;
            int y = 50 + i * 40;
            
            // Larger cloud shape
            for (int j = 0; j < 4; j++) {
                SDL_Rect cloud = {x + j * 35, y, 70, 45};
                SDL_RenderFillRect(renderer, &cloud);
            }
        }
    }
    
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
    // Draw clouds in background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 100);
    SDL_Rect cloud1 = {100, 80, 120, 60};
    SDL_Rect cloud2 = {900, 120, 150, 70};
    SDL_Rect cloud3 = {450, 50, 100, 50};
    SDL_RenderFillRect(renderer, &cloud1);
    SDL_RenderFillRect(renderer, &cloud2);
    SDL_RenderFillRect(renderer, &cloud3);
    
    // Title
    SDL_Color titleColor = {100, 255, 100, 255};
    renderText("HELICOPTER GAME", SCREEN_WIDTH/2, 150, fontLarge, titleColor, true);
    
    // Decorative helicopters
    int heliY = 280;
    
    // Left helicopter - green
    SDL_SetRenderDrawColor(renderer, 50, 160, 50, 255);
    SDL_Rect heli1Body = {200, heliY, 40, 15};
    SDL_RenderFillRect(renderer, &heli1Body);
    SDL_SetRenderDrawColor(renderer, 100, 200, 100, 255);
    SDL_Rect heli1Cockpit = {230, heliY - 5, 15, 20};
    SDL_RenderFillRect(renderer, &heli1Cockpit);
    
    // Right helicopter - red
    SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255);
    SDL_Rect heli2Body = {1000, heliY, 30, 12};
    SDL_RenderFillRect(renderer, &heli2Body);
    SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);
    SDL_Rect heli2Cockpit = {1005, heliY - 5, 12, 15};
    SDL_RenderFillRect(renderer, &heli2Cockpit);
    
    // Controls box
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 220);
    SDL_Rect controlBox = {SCREEN_WIDTH/2 - 300, 320, 600, 240};
    SDL_RenderFillRect(renderer, &controlBox);
    
    SDL_SetRenderDrawColor(renderer, 100, 200, 255, 255);
    SDL_Rect controlBorder = {SCREEN_WIDTH/2 - 303, 317, 606, 246};
    SDL_RenderDrawRect(renderer, &controlBorder);
    
    // Controls text
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color yellowColor = {255, 255, 100, 255};
    
    renderText("CONTROLS:", SCREEN_WIDTH/2, 340, fontMedium, yellowColor, true);
    renderText("UP ARROW - Thrust (Hold to rise)", SCREEN_WIDTH/2, 395, fontSmall, whiteColor, true);
    renderText("SPACE / X - Shoot", SCREEN_WIDTH/2, 435, fontSmall, whiteColor, true);
    renderText("P / ESC - Pause", SCREEN_WIDTH/2, 475, fontSmall, whiteColor, true);
    renderText("ESC - Quit (from menu)", SCREEN_WIDTH/2, 515, fontSmall, whiteColor, true);
    
    // Blinking start button
    static int blinkTimer = 0;
    blinkTimer = (blinkTimer + 1) % 60;
    
    if (blinkTimer < 40) {
        SDL_Color startColor = {100, 255, 100, 255};
        renderText("PRESS SPACE TO START", SCREEN_WIDTH/2, 600, fontMedium, startColor, true);
    }
    
    // High score
    if (highScore > 0) {
        SDL_Color scoreColor = {255, 255, 0, 255};
        char scoreText[100];
        snprintf(scoreText, sizeof(scoreText), "HIGH SCORE: %d", highScore);
        renderText(scoreText, SCREEN_WIDTH/2, 660, fontSmall, scoreColor, true);
    }
}

void Game::renderHUD() {
    // Semi-transparent dark panel for health - compact size with safe margins
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect healthPanel = {10, 10, 200, 45};
    SDL_RenderFillRect(renderer, &healthPanel);
    
    // Panel border
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
    SDL_RenderDrawRect(renderer, &healthPanel);
    
    // Health bar background (dark red)
    SDL_SetRenderDrawColor(renderer, 60, 0, 0, 255);
    SDL_Rect healthBg = {18, 18, 184, 25};
    SDL_RenderFillRect(renderer, &healthBg);
    
    // Health bar fill with color based on health level
    int health = player->getHealth();
    int healthWidth = static_cast<int>(health * 1.84f);  // Scale to 184px max
    
    // Color changes based on health
    if (health > 66) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green
    } else if (health > 33) {
        SDL_SetRenderDrawColor(renderer, 255, 200, 0, 255);  // Yellow
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red
        
        // Pulsing effect for low health
        static int pulseTimer = 0;
        pulseTimer = (pulseTimer + 1) % 30;
        if (pulseTimer < 15) {
            SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
        }
    }
    
    SDL_Rect healthFill = {18, 18, healthWidth, 25};
    SDL_RenderFillRect(renderer, &healthFill);
    
    // Health segments for visual feedback
    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 150);
    for (int i = 1; i < 10; i++) {
        int segmentX = 18 + i * 18;
        SDL_RenderDrawLine(renderer, segmentX, 18, segmentX, 43);
    }
    
    // Health bar border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &healthBg);
    
    // Health text with shadow - properly centered in panel
    SDL_Color shadowColor = {0, 0, 0, 255};
    SDL_Color whiteColor = {255, 255, 255, 255};
    char healthText[50];
    snprintf(healthText, sizeof(healthText), "HP: %d%%", health);
    renderText(healthText, 111, 31, fontSmall, shadowColor, true);  // Shadow
    renderText(healthText, 110, 30, fontSmall, whiteColor, true);   // Text
    
    // Stats panel (semi-transparent) - smaller size with safe margins
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect statsPanel = {SCREEN_WIDTH - 200, 10, 190, 95};
    SDL_RenderFillRect(renderer, &statsPanel);
    
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 200);
    SDL_RenderDrawRect(renderer, &statsPanel);
    
    // Score with shadow - safe positioning with 15px margin from panel edge
    SDL_Color yellowColor = {255, 255, 100, 255};
    char scoreText[100];
    snprintf(scoreText, sizeof(scoreText), "SCORE: %d", score);
    renderText(scoreText, SCREEN_WIDTH - 186, 23, fontSmall, shadowColor, false);
    renderText(scoreText, SCREEN_WIDTH - 185, 22, fontSmall, yellowColor, false);
    
    // Distance - safe positioning
    char distText[100];
    snprintf(distText, sizeof(distText), "DIST: %.0fm", distanceTraveled);
    renderText(distText, SCREEN_WIDTH - 186, 50, fontSmall, shadowColor, false);
    renderText(distText, SCREEN_WIDTH - 185, 49, fontSmall, whiteColor, false);
    
    // Kills - safe positioning
    char killText[100];
    snprintf(killText, sizeof(killText), "KILLS: %d", enemiesKilled);
    renderText(killText, SCREEN_WIDTH - 186, 77, fontSmall, shadowColor, false);
    renderText(killText, SCREEN_WIDTH - 185, 76, fontSmall, whiteColor, false);
}

void Game::renderGameOver() {
    // Dark overlay
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 180);
    SDL_Rect overlay = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &overlay);
    
    // Game Over box
    SDL_SetRenderDrawColor(renderer, 60, 0, 0, 240);
    SDL_Rect gameOverBox = {SCREEN_WIDTH/2 - 300, 180, 600, 380};
    SDL_RenderFillRect(renderer, &gameOverBox);
    
    SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255);
    SDL_Rect border = {SCREEN_WIDTH/2 - 305, 175, 610, 390};
    SDL_RenderDrawRect(renderer, &border);
    
    // Game Over text
    SDL_Color redColor = {255, 50, 50, 255};
    renderText("GAME OVER", SCREEN_WIDTH/2, 230, fontLarge, redColor, true);
    
    // Final stats
    SDL_Color whiteColor = {255, 255, 255, 255};
    SDL_Color yellowColor = {255, 255, 100, 255};
    
    char finalScore[100];
    snprintf(finalScore, sizeof(finalScore), "FINAL SCORE: %d", score);
    renderText(finalScore, SCREEN_WIDTH/2, 330, fontMedium, yellowColor, true);
    
    char distText[100];
    snprintf(distText, sizeof(distText), "DISTANCE: %.0fm", distanceTraveled);
    renderText(distText, SCREEN_WIDTH/2, 385, fontSmall, whiteColor, true);
    
    char  killText[100];
    snprintf(killText, sizeof(killText), "ENEMIES KILLED: %d", enemiesKilled);
    renderText(killText, SCREEN_WIDTH/2, 420, fontSmall, whiteColor, true);
    
    // High score notification
    if (score == highScore && score > 0) {
        SDL_Color greenColor = {100, 255, 100, 255};
        renderText("NEW HIGH SCORE!", SCREEN_WIDTH/2, 465, fontMedium, greenColor, true);
    } else if (highScore > 0) {
        char highScoreText[100];
        snprintf(highScoreText, sizeof(highScoreText), "HIGH SCORE: %d", highScore);
        renderText(highScoreText, SCREEN_WIDTH/2, 465, fontSmall, whiteColor, true);
    }
    
    // Instructions
    SDL_Color lightGreenColor = {150, 255, 150, 255};
    renderText("PRESS SPACE TO RESTART", SCREEN_WIDTH/2, 510, fontSmall, lightGreenColor, true);
    renderText("PRESS ESC FOR MENU", SCREEN_WIDTH/2, 540, fontSmall, whiteColor, true);
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
    
    // Close fonts
    if (fontLarge) {
        TTF_CloseFont(fontLarge);
        fontLarge = nullptr;
    }
    if (fontMedium) {
        TTF_CloseFont(fontMedium);
        fontMedium = nullptr;
    }
    if (fontSmall) {
        TTF_CloseFont(fontSmall);
        fontSmall = nullptr;
    }
    
    // Destroy SDL objects
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    
    TTF_Quit();
    SDL_Quit();
}

void Game::renderText(const char* text, int x, int y, TTF_Font* font, SDL_Color color, bool centered) {
    if (!font) return;  // Fallback if font not loaded
    
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    
    SDL_Rect destRect;
    destRect.w = surface->w;
    destRect.h = surface->h;
    
    if (centered) {
        destRect.x = x - surface->w / 2;
        destRect.y = y - surface->h / 2;
    } else {
        destRect.x = x;
        destRect.y = y;
    }
    
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
