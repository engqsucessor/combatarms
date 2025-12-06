#include "GameState.h"

GameState::GameState()
    : currentWave(0)
    , zombiesKilledThisWave(0)
    , zombiesToSpawnThisWave(0)
    , waveStartTime(0.0f)
    , currentTime(0.0f)
    , phase(GamePhase::WAITING_FOR_PLAYERS)
    , nextEntityId(1)
{
}

int GameState::addPlayer(const std::string& name, bool isAI) {
    int id = getNextEntityId();
    Vector2 spawnPos(500.0f, 500.0f); // Default spawn, should use map spawn points
    players[id] = std::make_unique<Player>(id, name, spawnPos, isAI);
    return id;
}

int GameState::addZombie(Vector2 spawnPos, ZombieType type) {
    int id = getNextEntityId();
    zombies[id] = std::make_unique<Zombie>(id, spawnPos, type);
    return id;
}

void GameState::removePlayer(int playerId) {
    players.erase(playerId);
}

void GameState::removeZombie(int zombieId) {
    zombies.erase(zombieId);
}

Player* GameState::getPlayer(int id) {
    auto it = players.find(id);
    return (it != players.end()) ? it->second.get() : nullptr;
}

Zombie* GameState::getZombie(int id) {
    auto it = zombies.find(id);
    return (it != zombies.end()) ? it->second.get() : nullptr;
}

void GameState::update(float deltaTime) {
    currentTime += deltaTime;

    // Update all players
    for (auto& pair : players) {
        pair.second->update(deltaTime);
    }

    // Update all zombies
    for (auto& pair : zombies) {
        pair.second->update(deltaTime);
    }

    // Check wave completion
    if (phase == GamePhase::WAVE_IN_PROGRESS) {
        checkWaveCompletion();
    }
}

void GameState::startWave() {
    currentWave++;
    zombiesKilledThisWave = 0;
    zombiesToSpawnThisWave = 10 + (currentWave * 5); // Increase zombies each wave
    waveStartTime = currentTime;
    phase = GamePhase::WAVE_IN_PROGRESS;
}

void GameState::completeWave() {
    phase = GamePhase::WAVE_COMPLETE;
}

void GameState::endGame() {
    phase = GamePhase::GAME_OVER;
}

int GameState::getAliveZombieCount() const {
    int count = 0;
    for (const auto& pair : zombies) {
        if (pair.second->isAlive()) {
            count++;
        }
    }
    return count;
}

int GameState::getAlivePlayerCount() const {
    int count = 0;
    for (const auto& pair : players) {
        if (pair.second->isAlive()) {
            count++;
        }
    }
    return count;
}

void GameState::checkWaveCompletion() {
    if (getAliveZombieCount() == 0 && zombiesKilledThisWave >= zombiesToSpawnThisWave) {
        completeWave();
    }

    // Game over if all players are dead
    if (getAlivePlayerCount() == 0) {
        endGame();
    }
}
