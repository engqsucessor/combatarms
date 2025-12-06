#pragma once

#include "GameState.h"
#include "Map.h"

class ZombieSpawner {
private:
    GameState* gameState;
    Map* map;

    float spawnInterval;
    float lastSpawnTime;
    int zombiesSpawnedThisWave;

public:
    ZombieSpawner(GameState* state, Map* gameMap);

    void update(float deltaTime);
    void startNewWave();

private:
    void spawnZombie();
    ZombieType selectZombieType();
};
