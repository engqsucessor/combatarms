#include "GameServer.h"
#include "AIController.h"
#include "ZombieSpawner.h"
#include <iostream>
#include <chrono>
#include <thread>

GameServer::GameServer(int port)
    : serverPort(port)
    , tickRate(1.0f / 60.0f) // 60 ticks per second
    , lastTickTime(0.0f)
    , running(false)
{
    map.loadCabinFeverMap();
    aiController = std::make_unique<AIController>(&gameState, &map);
    zombieSpawner = std::make_unique<ZombieSpawner>(&gameState, &map);
}

GameServer::~GameServer() {
    stop();
}

bool GameServer::start() {
    std::cout << "Starting Cabin Fever Server on port " << serverPort << "..." << std::endl;

    // Spawn AI teammates
    spawnAITeammates();

    // Start the game
    gameState.startWave();
    zombieSpawner->startNewWave();

    running = true;
    gameLoopThread = std::thread(&GameServer::gameLoop, this);

    std::cout << "Server started! Waiting for players..." << std::endl;
    return true;
}

void GameServer::stop() {
    if (running) {
        running = false;
        if (gameLoopThread.joinable()) {
            gameLoopThread.join();
        }
        std::cout << "Server stopped." << std::endl;
    }
}

void GameServer::run() {
    // Main server thread - handles network I/O
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS

        // In a real implementation, this would:
        // 1. Accept new connections
        // 2. Receive packets from clients
        // 3. Process input packets
        // 4. Send state updates

        // For now, it's a simple loop
    }
}

void GameServer::gameLoop() {
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        float deltaTime = elapsed.count();

        if (deltaTime >= tickRate) {
            updateGame(deltaTime);
            lastTime = currentTime;
        } else {
            // Sleep for remaining time
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

void GameServer::updateGame(float deltaTime) {
    // Update game state
    gameState.update(deltaTime);

    // Update AI
    aiController->updateAI(deltaTime);

    // Update zombie spawner
    zombieSpawner->update(deltaTime);

    // Check for wave completion
    if (gameState.getPhase() == GamePhase::WAVE_COMPLETE) {
        std::cout << "Wave " << gameState.getCurrentWave() << " completed!" << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        gameState.startWave();
        zombieSpawner->startNewWave();
    }

    // Check for game over
    if (gameState.getPhase() == GamePhase::GAME_OVER) {
        std::cout << "Game Over! Survived " << gameState.getCurrentWave() << " waves." << std::endl;
        running = false;
    }

    // Send updates to clients (in real implementation)
    // sendGameStateToClients();
}

void GameServer::processClientInputs() {
    // Process queued client inputs
}

void GameServer::sendGameStateToClients() {
    // Serialize and send game state to all connected clients
}

void GameServer::handleClientConnect(const std::string& clientAddress, const std::string& playerName) {
    Vector2 spawnPos = map.getRandomPlayerSpawn();
    int playerId = gameState.addPlayer(playerName, false);

    ConnectedClient client;
    client.address = clientAddress;
    client.playerId = playerId;
    client.lastHeartbeat = getCurrentTime();

    clients[clientAddress] = client;

    std::cout << "Player " << playerName << " connected (ID: " << playerId << ")" << std::endl;
}

void GameServer::handleClientDisconnect(const std::string& clientAddress) {
    auto it = clients.find(clientAddress);
    if (it != clients.end()) {
        gameState.removePlayer(it->second.playerId);
        clients.erase(it);
        std::cout << "Player disconnected" << std::endl;
    }
}

void GameServer::handleClientInput(const std::string& clientAddress, const Vector2& moveDir, const Vector2& aimTarget, bool shoot) {
    auto it = clients.find(clientAddress);
    if (it == clients.end()) return;

    Player* player = gameState.getPlayer(it->second.playerId);
    if (!player) return;

    // Apply player input
    player->move(moveDir, tickRate);
    player->aim(aimTarget);
    if (shoot) {
        player->shoot();
    }
}

void GameServer::spawnAITeammates() {
    for (int i = 0; i < NUM_AI_TEAMMATES; i++) {
        std::string aiName = "AI_Teammate_" + std::to_string(i + 1);
        Vector2 spawnPos = map.getRandomPlayerSpawn();
        gameState.addPlayer(aiName, true);
        std::cout << "Spawned " << aiName << std::endl;
    }
}

float GameServer::getCurrentTime() {
    return gameState.getCurrentTime();
}
