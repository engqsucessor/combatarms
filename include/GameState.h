#pragma once

#include "Player.h"
#include "Zombie.h"
#include <vector>
#include <memory>
#include <map>

enum class GamePhase {
    WAITING_FOR_PLAYERS,
    STARTING,
    WAVE_IN_PROGRESS,
    WAVE_COMPLETE,
    GAME_OVER
};

class GameState {
private:
    std::map<int, std::unique_ptr<Player>> players;
    std::map<int, std::unique_ptr<Zombie>> zombies;

    int currentWave;
    int zombiesKilledThisWave;
    int zombiesToSpawnThisWave;
    float waveStartTime;
    float currentTime;
    GamePhase phase;

    int nextEntityId;

public:
    GameState();

    // Entity management
    int addPlayer(const std::string& name, bool isAI = false);
    int addZombie(Vector2 spawnPos, ZombieType type = ZombieType::NORMAL);
    void removePlayer(int playerId);
    void removeZombie(int zombieId);

    Player* getPlayer(int id);
    Zombie* getZombie(int id);

    const std::map<int, std::unique_ptr<Player>>& getPlayers() const { return players; }
    const std::map<int, std::unique_ptr<Zombie>>& getZombies() const { return zombies; }

    // Game flow
    void update(float deltaTime);
    void startWave();
    void completeWave();
    void endGame();

    // Getters
    int getCurrentWave() const { return currentWave; }
    GamePhase getPhase() const { return phase; }
    float getCurrentTime() const { return currentTime; }
    int getAliveZombieCount() const;
    int getAlivePlayerCount() const;

private:
    int getNextEntityId() { return nextEntityId++; }
    void checkWaveCompletion();
};
