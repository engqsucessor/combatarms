# Combat Arms: Cabin Fever

A C++ recreation of the classic Combat Arms Cabin Fever zombie survival game mode.

## Features

- **Zombie Survival Gameplay**: Fight waves of increasingly difficult zombies
- **AI Teammates**: 3 AI-controlled teammates fight alongside you
- **Multiple Zombie Types**: Normal, Fast, Heavy, and Boss zombies
- **Weapon System**: Various weapons with realistic mechanics
- **Cabin Fever Map**: Classic small cabin map with strategic chokepoints
- **Server/Client Architecture**: Ready for multiplayer (offline mode currently implemented)

## Building

### Prerequisites

- CMake 3.15 or higher
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- SDL2 development libraries

#### Installing SDL2 on Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install libsdl2-dev cmake build-essential
```

#### Installing SDL2 on macOS:
```bash
brew install sdl2 cmake
```

#### Installing SDL2 on Windows:
Download SDL2 development libraries from https://www.libsdl.org/download-2.0.php

### Build Instructions

```bash
# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make -j$(nproc)
```

Or use the provided build script:
```bash
chmod +x build.sh
./build.sh
```

## Running the Game

### Client (Offline Mode)
```bash
./build/cabin_fever_client [player_name]
```

### Dedicated Server
```bash
./build/cabin_fever_server [port]
```

## Controls

- **WASD / Arrow Keys**: Move
- **Mouse**: Aim
- **Left Click**: Shoot
- **R**: Reload
- **ESC**: Quit

## Game Mechanics

### Waves
- Each wave spawns more zombies than the last
- Zombie spawn rate increases with wave number
- Short break between waves to prepare

### Zombie Types
- **Normal**: Standard zombie with balanced stats
- **Fast**: Quick but weak zombies (appears from wave 3+)
- **Heavy**: Slow but strong zombies (appears from wave 5+)
- **Boss**: Powerful zombies with high health (appears from wave 10+)

### Weapons
- **M4A1 Assault Rifle**: Default balanced weapon
- **MP5 SMG**: High fire rate, lower damage
- **Shotgun**: High damage, close range
- **Sniper Rifle**: Very high damage, long range
- **Pistol**: Backup weapon

## Architecture

The project is split into three main components:

1. **Common Library** (`src/common/`): Shared game logic, entities, and systems
2. **Client** (`src/client/`): SDL2-based rendering and input handling
3. **Server** (`src/server/`): Game state management, AI, and wave spawning

## Future Enhancements

- Full multiplayer networking implementation
- More maps
- More weapon varieties
- Power-ups and perks
- Player progression system
- Sound effects and music
- Better graphics and animations

## License

This is a fan recreation for educational purposes. Combat Arms is a trademark of Nexon.

## Contributing

Feel free to submit pull requests or open issues for bugs and feature requests!
