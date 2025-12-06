# 🚁 Helicopter Game

A classic 2D side-scrolling helicopter game built with C++ and SDL2. Navigate your helicopter through procedurally generated terrain, avoid obstacles, and shoot down enemy aircraft!

## 📋 Features

- **Physics-Based Flight Controls**: Hold thrust to rise, release to fall with realistic gravity
- **Procedural Terrain Generation**: Infinite scrolling landscape with dynamic ground and ceiling
- **Combat System**: Shoot bullets to destroy enemy helicopters
- **Multiple Enemy Types**: 
  - Stationary enemies
  - Sine wave pattern fliers
  - Charging enemies that track your position
- **Dynamic Obstacles**:
  - Static barriers
  - Vertically moving platforms
  - Rotating hazards
- **Particle Effects**: Explosions when enemies are destroyed
- **Score System**: Earn points for distance traveled and enemies killed
- **High Score Persistence**: Your best score is saved between sessions
- **Game States**: Main menu, gameplay, pause, and game over screens

## 🎮 Controls

| Key | Action |
|-----|--------|
| **SPACE** / **UP ARROW** | Apply thrust (hold to rise, release to fall) |
| **SPACE** / **X** | Shoot bullets |
| **P** / **ESC** | Pause game |
| **ESC** | Quit to menu |

## 🛠️ Prerequisites

You need to have SDL2 and its extension libraries installed on your system.

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev
```

### Fedora/RHEL
```bash
sudo dnf install SDL2-devel SDL2_image-devel SDL2_ttf-devel SDL2_mixer-devel
```

### macOS (using Homebrew)
```bash
brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

### Arch Linux
```bash
sudo pacman -S sdl2 sdl2_image sdl2_ttf sdl2_mixer
```

## 🔨 Building the Game

1. **Clone or download this repository**

2. **Build using Make**:
   ```bash
   make
   ```

3. **Run the game**:
   ```bash
   ./helicopter_game
   ```

### Alternative Commands

```bash
# Clean build artifacts
make clean

# Build in debug mode (with symbols)
make debug

# Build and run immediately
make run
```

## 📁 Project Structure

```
Helicopter-game/
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Main game logic
│   ├── Helicopter.cpp     # Player helicopter
│   ├── Bullet.cpp         # Projectiles
│   ├── Enemy.cpp          # Enemy aircraft
│   ├── Obstacle.cpp       # Obstacles
│   ├── Terrain.cpp        # Scrolling terrain
│   ├── Particle.cpp       # Explosion effects
│   └── CollisionDetector.cpp  # Collision utilities
├── include/               # Header files
│   ├── Game.h
│   ├── Helicopter.h
│   ├── Bullet.h
│   ├── Enemy.h
│   ├── Obstacle.h
│   ├── Terrain.h
│   ├── Particle.h
│   └── CollisionDetector.h
├── assets/                # Game assets directory
├── Makefile              # Build configuration
└── README.md             # This file
```

## 🎯 Gameplay

### Objective
Fly your helicopter as far as possible while avoiding:
- The procedurally generated terrain (ground and ceiling)
- Static and moving obstacles
- Enemy helicopters

### Scoring
- **Distance**: Earn points for every unit of distance traveled
- **Combat**: Earn 100 points for each enemy destroyed
- **High Score**: Beat your personal best!

### Tips
- Timing is everything - learn when to hold thrust and when to let gravity take over
- Shoot enemies from a distance to avoid collisions
- Watch for moving obstacles - they can be tricky!
- The terrain is procedurally generated, so every game is unique

## 🏗️ Technical Details

### Technologies Used
- **C++17**: Modern C++ features
- **SDL2**: Simple DirectMedia Layer for graphics and input
- **Procedural Generation**: Noise-based terrain generation
- **Physics Simulation**: Gravity and velocity-based movement
- **AABB Collision Detection**: Accurate bounding box collisions

### Architecture
- **Entity-Component Pattern**: Each game object (helicopter, enemies, obstacles) is a separate entity
- **Game State Machine**: Clean separation between menu, playing, paused, and game over states
- **Frame-Independent Physics**: Consistent 60 FPS gameplay

## 🐛 Troubleshooting

### SDL2 Not Found
If you get compilation errors about missing SDL2:
```bash
# Check if SDL2 is installed
pkg-config --modversion sdl2

# If not found, install it using the package manager commands above
```

### Linker Errors
Make sure all SDL2 libraries are installed:
- `libsdl2-dev` (or equivalent)
- `libsdl2-image-dev`
- `libsdl2-ttf-dev`
- `libsdl2-mixer-dev`

### Game Runs Too Fast/Slow
The game is designed to run at 60 FPS with VSync. If you experience performance issues:
- Check your graphics drivers
- Try lowering screen resolution (modify `SCREEN_WIDTH` and `SCREEN_HEIGHT` in `Game.h`)

## 🔮 Future Enhancements

Potential features for future versions:
- [ ] Sound effects and background music
- [ ] Power-ups (shields, rapid fire, etc.)
- [ ] Multiple levels with increasing difficulty
- [ ] Different helicopter types to choose from
- [ ] Boss battles
- [ ] Multiplayer mode
- [ ] Sprite-based graphics instead of geometric shapes
- [ ] Level editor

## 📜 License

This project is open source and available for educational purposes.

## 🤝 Contributing

Feel free to fork this project and add your own features! Some ideas:
- Add new enemy types with unique behaviors
- Implement sound effects using SDL_mixer
- Create sprite assets for better visuals
- Add new obstacle types
- Implement a difficulty curve system

## 👨‍💻 Author

Created as a demonstration of 2D game development with C++ and SDL2.

---

**Enjoy the game and happy flying! 🚁**
