# Combat Arms: Cabin Fever - Browser FPS

A playable-in-browser recreation of the classic Combat Arms Cabin Fever zombie survival mode using React + Three.js.

## 🎮 Features

### ✅ Currently Implemented

**Player Systems:**
- First-person controls with WASD movement
- Pointer lock camera (mouse look)
- Sprint (Shift), Crouch (Ctrl), Jump (Space)
- HP (100) + AP (100) system with armor absorption
- Physics-based movement with Rapier

**Weapon System:**
- M4A1 assault rifle
- Raycast shooting with hit detection
- Recoil patterns (vertical + random horizontal)
- Reload system (R key, 2 second animation)
- Magazine (30) + Reserve ammo (90)
- Bullet holes on impact
- Muzzle flash effects

**Map:**
- Cabin Fever layout with wooden walls
- Doorways and cover objects (crates)
- Collision detection
- Shadows and lighting

**UI/HUD:**
- Health and armor bars (bottom left)
- Ammo counter (bottom right)
- Crosshair (center)
- Wave info (top center)
- Kill counter
- Control instructions

**Game Mode:**
- Wave-based spawning system
- Score tracking
- Zustand state management

### 🚧 Coming Soon
- [ ] Zombie AI with pathfinding
- [ ] Wave spawner with increasing difficulty
- [ ] More weapons (AK-47, shotgun, sniper)
- [ ] Audio system (gunshots, footsteps, ambient)
- [ ] Visual effects (blood, smoke, particles)
- [ ] Better 3D models (currently using primitives)
- [ ] Multiplayer with Socket.io

## 🛠️ Tech Stack

- **React 18** - UI framework
- **Three.js** - 3D rendering
- **React Three Fiber** - React renderer for Three.js
- **@react-three/drei** - Useful helpers
- **@react-three/rapier** - Physics engine (WASM)
- **Zustand** - State management
- **Vite** - Build tool and dev server

## 📁 Project Structure

```
src/
├── components/
│   ├── Player.jsx           # FPS controller with physics
│   ├── Weapon.jsx           # Shooting mechanics
│   ├── Map.jsx              # Cabin Fever map layout
│   └── UI/
│       ├── GameHUD.jsx      # In-game HUD overlay
│       ├── GameHUD.css
│       ├── MainMenu.jsx     # Start screen
│       └── MainMenu.css
├── stores/
│   └── gameStore.js         # Zustand global state
├── hooks/
│   └── useKeyboardControls.js
├── Game.jsx                 # Main 3D scene
├── App.jsx                  # App entry point
└── main.jsx                 # React entry
```

## 🚀 Quick Start

### Install Dependencies
```bash
npm install
```

### Run Development Server
```bash
npm run dev
```

Then open http://localhost:3000

### Build for Production
```bash
npm run build
npm run preview
```

## 🎯 How to Play

1. Click **START GAME** in the main menu
2. Click anywhere in the game to lock your mouse
3. Use WASD to move, mouse to aim
4. Left click to shoot
5. Press R to reload
6. Press ESC to unlock mouse and pause

## ⌨️ Controls

| Action | Key |
|--------|-----|
| Move | WASD or Arrow Keys |
| Sprint | Left Shift |
| Crouch | Left Ctrl |
| Jump | Space |
| Look Around | Mouse |
| Shoot | Left Click |
| Reload | R |
| Pause/Unlock Mouse | ESC |

## 🎨 Features Showcase

### HP/AP System
Just like Combat Arms:
- 100 HP (red bar)
- 100 AP armor (blue bar)
- Armor absorbs 50% of damage
- When armor depletes, full damage to health

### Weapon Mechanics
- Realistic recoil patterns
- Bullet spread (improved when aiming)
- Magazine system with reserves
- 2-second reload animation
- Visual bullet holes on impact

### Combat Arms Authentic HUD
- Health/Armor bars bottom left
- Ammo counter bottom right (30/90 format)
- Wave counter top center
- Kill counter top left
- Clean crosshair design

## 🔧 Development Roadmap

**Phase 1: Core Gameplay** ✅
- [x] FPS controls
- [x] Weapon shooting
- [x] HUD
- [x] Basic map

**Phase 2: AI & Combat** (In Progress)
- [ ] Zombie AI with navmesh
- [ ] Wave spawning system
- [ ] Health/damage system
- [ ] Kill tracking

**Phase 3: Content**
- [ ] More weapons (AK-47, shotgun, sniper)
- [ ] Weapon switching (1-5 keys)
- [ ] Different zombie types
- [ ] Better Cabin Fever map

**Phase 4: Polish**
- [ ] Sound effects (Howler.js)
- [ ] Visual effects (particles, blood)
- [ ] Animations
- [ ] Post-processing

**Phase 5: Multiplayer**
- [ ] Socket.io server
- [ ] Player sync
- [ ] Leaderboards

## 🌐 Deployment

Deploy to Vercel, Netlify, or GitHub Pages:

```bash
npm run build
# Upload 'dist' folder
```

The game runs entirely in the browser - no installation needed!

## 🤝 Contributing

Want to add features? PRs welcome!

Ideas:
- Add more zombie types
- Implement power-ups
- Create new maps
- Add weapon skins
- Improve AI behavior

## 📝 License

Fan project for educational purposes. Combat Arms is owned by Nexon.

---

**Play it in your browser. No downloads, no install. Just FPS action! 🎮**
