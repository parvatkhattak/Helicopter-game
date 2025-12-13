# 🚁 Helicopter Game

A polished 2D side-scrolling helicopter game built with C++ and SDL2. Navigate your helicopter through procedurally generated terrain, avoid obstacles, and shoot down enemy aircraft with stunning visual effects!

## ✨ Latest Updates

- **Enhanced UI Realism**: Gradient sky backgrounds, parallax clouds, modern semi-transparent HUD panels
- **Professional Text Rendering**: SDL_ttf integration for crisp, readable text throughout the game
- **Color-Coded Health System**: Health bar changes from green → yellow → red with pulsing warning at low health
- **Atmospheric Graphics**: Multi-layer terrain with depth, improved shadows, realistic cloud movement
- **Comfortable Menu**: Dark blue gradient background easy on the eyes

## 📋 Features

### Visual & Graphics
- **Realistic Sky Rendering**: Beautiful gradient sky from deep blue to horizon light
- **Parallax Cloud System**: Multi-layer clouds moving at different speeds for depth
- **Modern HUD**: Semi-transparent panels with text shadows for excellent readability
- **Enhanced Helicopter Graphics**: Detailed rendering with shadows, gradient colors, animated rotors, thrust effects
- **Enemy Helicopters**: Detailed red aircraft with warning lights and rotor animations
- **Multi-Layer Terrain**: Grass, dirt, and rock layers with realistic colors and depth
- **3D-Style Obstacles**: Stone blocks, energy barriers, and hazard-striped rotating obstacles
- **Particle Effects**: Explosions with varied sizes and colors when enemies are destroyed

### Gameplay
- **Physics-Based Flight Controls**: Hold UP ARROW to rise, release to fall with realistic gravity
- **Combat System**: Shoot bullets with SPACE/X keys to destroy enemy helicopters
- **Multiple Enemy Types**: 
  - Stationary enemies
  - Sine wave pattern fliers
  - Charging enemies that track your position with warning lights
- **Dynamic Obstacles**:
  - Static stone barriers with 3D shading
  - Vertically moving purple energy platforms with glow effects
  - Rotating orange hazard obstacles with warning stripes
- **Procedural Terrain Generation**: Infinite scrolling landscape with dynamic ground and ceiling
- **Score System**: Earn points for distance traveled and enemies killed
- **High Score Persistence**: Your best score is saved between sessions
- **Game States**: Main menu, gameplay, pause, and game over screens with smooth transitions

## 🎮 Controls

| Key | Action |
|-----|--------|
| **UP ARROW** | Apply thrust (hold to rise, release to fall) |
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

### Automated Installation
A helper script is provided for multiple Linux distributions:
```bash
chmod +x install_sdl2.sh
./install_sdl2.sh
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

# Rebuild everything
make clean && make

# The game will auto-detect system fonts for text rendering
```

## 📁 Project Structure

```
Helicopter-game/
├── src/                    # Source files
│   ├── main.cpp           # Entry point
│   ├── Game.cpp           # Main game logic and rendering
│   ├── Helicopter.cpp     # Player helicopter with physics
│   ├── Bullet.cpp         # Projectiles
│   ├── Enemy.cpp          # Enemy aircraft with AI
│   ├── Obstacle.cpp       # Dynamic obstacles
│   ├── Terrain.cpp        # Procedural terrain generation
│   ├── Particle.cpp       # Explosion particle effects
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
├── assets/                # Game assets directory (future use)
├── Makefile              # Build configuration
├── install_sdl2.sh       # SDL2 installation helper
├── highscore.txt         # Persistent high score storage
└── README.md             # This file
```

## 🎯 Gameplay

### Objective
Fly your helicopter as far as possible while avoiding:
- The procedurally generated terrain (ground and ceiling)
- Static and moving obstacles
- Enemy helicopters

### HUD Information
- **HP Bar**: Color-coded health (green/yellow/red) with pulsing warning at low health
- **Score**: Running total of points earned
- **Distance**: How far you've traveled in meters
- **Kills**: Number of enemies destroyed

### Scoring
- **Distance**: Earn points for every unit of distance traveled
- **Combat**: Earn 100 points for each enemy destroyed
- **High Score**: Beat your personal best!

### Tips
- Timing is everything - learn when to hold thrust and when to let gravity take over
- Watch your health bar - it changes color to warn you of danger
- Shoot enemies from a distance to avoid collisions
- Watch for moving obstacles - they can be tricky!
- The terrain is procedurally generated, so every game is unique
- Purple energy barriers move vertically - time your passage carefully

## 🏗️ Technical Details

### Technologies Used
- **C++17**: Modern C++ features
- **SDL2**: Graphics rendering and window management
- **SDL2_ttf**: Professional text rendering with system fonts
- **SDL2_image**: Image loading support (framework ready)
- **SDL2_mixer**: Audio support (framework ready)
- **Procedural Generation**: Noise-based terrain generation
- **Physics Simulation**: Gravity and velocity-based movement
- **AABB Collision Detection**: Accurate bounding box collisions

### Visual Features
- **Gradient Sky**: Programmatic sky rendering with smooth color transitions
- **Parallax Scrolling**: Multiple cloud layers for depth perception
- **Alpha Blending**: Semi-transparent UI panels and particle effects
- **Color-Coded Feedback**: Visual health warnings and state indicators
- **Text Shadows**: Enhanced readability on varying backgrounds

### Architecture
- **Entity-Component Pattern**: Each game object is a separate, self-contained entity
- **Game State Machine**: Clean separation between menu, playing, paused, and game over states
- **Frame-Independent Physics**: Consistent 60 FPS gameplay with VSync
- **Modular Design**: Easy to extend with new features and game objects

## 🐛 Troubleshooting

### SDL2 Not Found
If you get compilation errors about missing SDL2:
```bash
# Check if SDL2 is installed
pkg-config --modversion sdl2

# If not found, install it using the package manager commands above
```

### Font Warning
If you see "Could not load fonts!" warning:
- The game will still run but text won't be displayed
- Install DejaVu or Liberation fonts on your system
- Most Linux distributions include these by default

### Linker Errors
Make sure all SDL2 libraries are installed:
- `libsdl2-dev` (or equivalent)
- `libsdl2-image-dev`
- `libsdl2-ttf-dev`
- `libsdl2-mixer-dev`

### Game Runs Too Fast/Slow
The game is designed to run at 60 FPS with VSync. If you experience performance issues:
- Check your graphics drivers
- The gradient rendering may be intensive on older hardware
- Try lowering screen resolution (modify `SCREEN_WIDTH` and `SCREEN_HEIGHT` in `Game.h`)

## 🔮 Completed Features

Recent additions to the game:
- ✅ Professional text rendering with SDL_ttf
- ✅ Gradient sky backgrounds with atmospheric effects
- ✅ Parallax cloud system
- ✅ Modern semi-transparent HUD with panels
- ✅ Color-coded health warnings
- ✅ Enhanced graphics for all game entities
- ✅ Comfortable dark menu background
- ✅ Text shadows for better readability
- ✅ Segmented health bar
- ✅ Multi-layer terrain rendering

### Future Enhancements

Potential features for future versions:
- [ ] Sound effects and background music (framework ready)
- [ ] Power-ups (shields, rapid fire, etc.)
- [ ] Multiple levels with increasing difficulty
- [ ] Different helicopter types to choose from
- [ ] Boss battles
- [ ] Multiplayer mode
- [ ] Sprite-based graphics for even better visuals
- [ ] Level editor
- [ ] Achievement system

## 📜 License

This project is open source and available for educational purposes.

## 🤝 Contributing

Feel free to fork this project and add your own features! Some ideas:
- Add new enemy types with unique behaviors
- Implement sound effects using SDL_mixer
- Create sprite assets for even better visuals
- Add new obstacle types
- Implement a difficulty curve system
- Add more visual effects (screen shake, motion blur)

## 👨‍💻 Development

This game has been continuously improved with:
- Enhanced visual realism and polished UI
- Professional text rendering system
- Comfortable, eye-friendly color schemes
- Modern game development practices
- Clean, maintainable C++ code

---

**Enjoy the game and happy flying! 🚁✨**
