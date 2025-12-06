#pragma once

#include "GameState.h"
#include "Map.h"
#include <string>
#include <map>
#include <thread>
#include <atomic>

class AIController;
class ZombieSpawner;

struct ConnectedClient {
    std::string address;
    int playerId;
    float lastHeartbeat;
};

class GameServer {
private:
    GameState gameState;
    Map map;
    std::map<std::string, ConnectedClient> clients;

    std::unique_ptr<AIController> aiController;
    std::unique_ptr<ZombieSpawner> zombieSpawner;

    std::atomic<bool> running;
    std::thread gameLoopThread;

    int serverPort;
    float tickRate;
    float lastTickTime;

    // AI teammates
    static const int NUM_AI_TEAMMATES = 3;

public:
    GameServer(int port = 7777);
    ~GameServer();

    bool start();
    void stop();
    void run();

private:
    void gameLoop();
    void processClientInputs();
    void updateGame(float deltaTime);
    void sendGameStateToClients();

    void handleClientConnect(const std::string& clientAddress, const std::string& playerName);
    void handleClientDisconnect(const std::string& clientAddress);
    void handleClientInput(const std::string& clientAddress, const Vector2& moveDir, const Vector2& aimTarget, bool shoot);

    void spawnAITeammates();
    float getCurrentTime();
};
