# Combat Arms: Cabin Fever - Unity Remake

A faithful recreation of the classic Combat Arms Cabin Fever zombie survival mode in Unity.

## Features

### Player Systems
- **FPS Controller**: WASD movement, sprint (Shift), crouch (Ctrl), jump
- **Mouse Look**: Smooth camera control with sensitivity settings
- **HP/AP System**: 100 HP + 100 AP with armor absorption mechanics
- **Stamina System**: Sprint drains stamina, regenerates when not sprinting
- **Hit Detection**: Headshot (2x), body (1x), arms (0.85x), legs (0.75x) multipliers

### Weapon Systems
- **Realistic Shooting**: Hitscan with spread, recoil patterns
- **ADS (Aim Down Sights)**: Reduced spread when aiming
- **Ammo Management**: Magazine + reserve ammo, realistic reload times
- **Multiple Weapons**:
  - M4A1: 35 damage, 750 RPM, balanced
  - AK-47: 42 damage, 600 RPM, high recoil
  - SCAR-L: 38 damage, 650 RPM, low recoil

### Cabin Fever Game Mode
- **Wave System**: Increasing difficulty each wave
- **Dynamic Spawning**: Zombies spawn progressively
- **Prep Time**: 30 seconds between waves
- **Score Tracking**: Points for kills and objectives
- **Scaling Difficulty**: More zombies + more health per wave

### Zombie AI
- **NavMesh Pathfinding**: Smart navigation around obstacles
- **Target Detection**: Finds nearest player within range
- **Chase Behavior**: Speeds up when chasing
- **Attack System**: Melee attacks with cooldown
- **Health Scaling**: Gets tougher each wave

## Project Structure

```
Assets/
├── Scripts/
│   ├── Player/
│   │   ├── FPSController.cs      # Movement and camera
│   │   └── PlayerHealth.cs       # HP/AP system
│   ├── Weapons/
│   │   ├── WeaponBase.cs         # Base weapon class
│   │   └── AssaultRifle.cs       # AR implementations
│   ├── Zombie/
│   │   ├── ZombieAI.cs           # AI behavior
│   │   └── ZombieHealth.cs       # Health system
│   ├── GameMode/
│   │   └── CabinFeverGameMode.cs # Wave spawning
│   └── UI/
│       └── GameHUD.cs            # HUD display
├── Scenes/
│   └── CabinFever.unity          # Main game scene
└── Prefabs/
    ├── Player.prefab
    └── Zombie.prefab
```

## Setup Instructions

### 1. Unity Version
- Unity 2022.3 LTS or newer recommended

### 2. Required Packages
- TextMeshPro (for UI)
- AI Navigation (NavMesh)

### 3. Scene Setup

**Create Player:**
1. Create Empty GameObject named "Player"
2. Add CharacterController component
3. Add FPSController script
4. Add PlayerHealth script
5. Add Camera as child, position at (0, 0.6, 0)

**Create Weapon:**
1. Add AssaultRifle.CreateM4A1() in Player's Start() method
2. Or create weapon GameObject and attach AssaultRifle script

**Create Zombie:**
1. Create Capsule GameObject named "Zombie"
2. Add NavMeshAgent component
3. Add ZombieAI script
4. Add ZombieHealth script
5. Set Layer to "Enemy"

**Setup Scene:**
1. Create ground plane
2. Bake NavMesh (Window → AI → Navigation)
3. Add CabinFeverGameMode to empty GameObject
4. Create spawn points (Empty GameObjects)
5. Assign spawn points to CabinFeverGameMode

**Setup UI:**
1. Create Canvas
2. Add health/armor bars (UI Images)
3. Add ammo text (TextMeshPro)
4. Add wave info text
5. Add GameHUD script to Canvas

### 4. Layer Setup
- Create "Player" layer
- Create "Enemy" layer
- Create "Ground" layer
- Assign to respective objects

### 5. Input Setup
- Edit → Project Settings → Input Manager
- Ensure "Fire1" (Mouse 0), "Fire2" (Mouse 1), "Jump" are configured

## Controls

- **WASD**: Move
- **Shift**: Sprint
- **Ctrl**: Crouch
- **Space**: Jump
- **Mouse**: Look around
- **Left Click**: Shoot
- **Right Click**: Aim Down Sights
- **R**: Reload

## Upcoming Features

- [ ] More weapon types (SMGs, Shotguns, Snipers)
- [ ] Weapon attachments system
- [ ] Multiple zombie types (Fast, Heavy, Boss)
- [ ] Cabin Fever map layout
- [ ] Sound effects and music
- [ ] Weapon models and animations
- [ ] Multiplayer support
- [ ] Leaderboards and stats

## Development Notes

This is the foundation - all core systems are in place. Next steps:
1. Create 3D models or use placeholders
2. Add animations
3. Build the Cabin Fever map
4. Add visual effects
5. Implement sound

## License

Fan project for educational purposes. Combat Arms is owned by Nexon.
