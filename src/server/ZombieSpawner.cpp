#include "ZombieSpawner.h"
#include <cstdlib>

ZombieSpawner::ZombieSpawner(GameState* state, Map* gameMap)
    : gameState(state)
    , map(gameMap)
    , spawnInterval(2.0f)
    , lastSpawnTime(0.0f)
    , zombiesSpawnedThisWave(0)
{
}

void ZombieSpawner::update(float deltaTime) {
    if (gameState->getPhase() != GamePhase::WAVE_IN_PROGRESS) {
        return;
    }

    float currentTime = gameState->getCurrentTime();

    // Check if it's time to spawn
    if (currentTime - lastSpawnTime >= spawnInterval) {
        int totalZombiesForWave = 10 + (gameState->getCurrentWave() * 5);

        if (zombiesSpawnedThisWave < totalZombiesForWave) {
            spawnZombie();
            lastSpawnTime = currentTime;
        }
    }
}

void ZombieSpawner::startNewWave() {
    zombiesSpawnedThisWave = 0;
    lastSpawnTime = gameState->getCurrentTime();

    // Adjust spawn rate based on wave
    int wave = gameState->getCurrentWave();
    spawnInterval = 2.0f - (wave * 0.1f);
    if (spawnInterval < 0.5f) {
        spawnInterval = 0.5f;
    }
}

void ZombieSpawner::spawnZombie() {
    Vector2 spawnPos = map->getRandomZombieSpawn();
    ZombieType type = selectZombieType();

    int zombieId = gameState->addZombie(spawnPos, type);
    zombiesSpawnedThisWave++;

    // Find a player to target
    const auto& players = gameState->getPlayers();
    if (!players.empty()) {
        auto it = players.begin();
        std::advance(it, std::rand() % players.size());
        Player* targetPlayer = it->second.get();

        Zombie* zombie = gameState->getZombie(zombieId);
        if (zombie && targetPlayer) {
            zombie->setTarget(targetPlayer->getPosition(), targetPlayer->getId());
        }
    }
}

ZombieType ZombieSpawner::selectZombieType() {
    int wave = gameState->getCurrentWave();
    int random = std::rand() % 100;

    // Increase difficulty with waves
    if (wave >= 10 && random < 5) {
        return ZombieType::BOSS;
    } else if (wave >= 5 && random < 20) {
        return ZombieType::HEAVY;
    } else if (wave >= 3 && random < 40) {
        return ZombieType::FAST;
    } else {
        return ZombieType::NORMAL;
    }
}
