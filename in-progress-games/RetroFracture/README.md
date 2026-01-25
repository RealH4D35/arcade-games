# RetroFracture 🎮

*A Multi-Genre Arcade Adventure*

RetroFracture is a multi-genre arcade game built in C++ using SplashKit, featuring 8-bit neon aesthetics and genre-switching gameplay. Each level represents a different genre, with the player adapting to new mechanics, controls, and challenges as they progress.

![RetroFracture Banner](assets/envs/Apartment%20Hallway.png)

## 🌟 Features

### 🎯 Genre-Switching Gameplay
- **Platformer**: Precision jumping and exploration
- **Run & Slash**: Melee combat with acquired weapons  
- **Beat 'Em Up**: Combo-based boss battles
- **Top-Down Shmup**: Vehicle combat

### 🎨 Visual Style
- **8-bit pixel art** with vibrant neon palette
- **640×480 resolution** (arcade-perfect)
- **Smooth animations** and dynamic camera

### 🕹️ Current Implementation
- Explorable hallway level with multiple apartments
- Apartment 101 interior (Actual interior to be implimented)
- Player physics and collision
- Intractable (door, for now) system with prompts
- Adaptive HUD

## 🚀 Quick Start

### Prerequisites
- C++17 compatible compiler (GCC, Clang, or MSVC)
- [SplashKit SDK](https://splashkit.io/) installed

### Installation & Building
```bash
# Fork and clone the repository
git clone https://github.com/yourusername/RetroFracture.git
cd RetroFracture

# Build with SplashKit
skm clang++ program.cpp src/*.cpp -o RetroFracture

# Launch the game!
./RetroFracture
```

### Controls
| Key | Action |
|-----|--------|
| **A/D** | Move left/right |
| **W** | Jump |
| **E** | Interact with objects |
| **ESC** | Quit game |
| **R** | Reset position |

## 📁 Project Structure

```
RetroFracture/
├── assets/                   # All game assets
│   ├── envs/                 # Backgrounds & environments
│   └── sprites/              # Player animations
├── src/                      # Source code
│   ├── game.cpp/.h           # Main game controller
│   ├── player.cpp/.h         # Player character & physics
│   ├── hallwaylevel.cpp/.h   # Hallway level logic
│   ├── lvl1.cpp/.h           # Apartment 101 level
│   ├── hud.cpp/.h            # UI & display
│   └── interactable.cpp/.h   # Interactive objects
├── program.cpp               # Entry point
└── config.txt                # Arcade mechine config
```

### Architecture Overview
- **Game Class**: Orchestrates levels, camera, and state
- **Player Class**: Handles movement, animation, and physics
- **Level Classes**: Modular level implementations
- **HUD Class**: Context-aware UI rendering
- **Interactable System**: Event-driven interactions

### Adding New Levels
1. Create `lvlX.cpp/.h` following existing patterns
2. Add to `Game` enum and instance variables
3. Implement transition methods
4. Add to update/draw switches

## 🗺️ Planned Roadmap

### Campaign (8 Levels, 2 Levels per Act)
```
Act 1:
├── ✅ Apartment Hallway
├── 🔄 Apartments (101 - 107)
│	- Apartment 101 - Player apartment
│	- Apartment 102 - Annoyed neighbor 1 apartment
│	- Apartment 103 - Annoyed neighbor 2 apartment
│	- Apartment 104 - Abandoned apartment
│	- Apartment 105 - Angry neighbor apartment
│	- Apartment 106 - Enemy apartment 1 (Small)
│	- Apartment 107 & 108 - Enemy apartment 2 (Big)
└── 📋 Rest of apartment complex

Act 2:
└── To be decided
Act 3:
└── To be decided
Act 4:
└── To be decided
```

### Coming Soon
- Enemy AI and combat systems
- Character selection mechanics
- Music and sound implementation
- More apartment interiors
- Lighting/shading effects

### Guidelines
- Follow existing code style and structure
- Document complex logic with comments
