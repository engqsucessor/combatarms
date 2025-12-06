#pragma once

#include "GameState.h"
#include "Map.h"
#include "Renderer.h"
#include "InputHandler.h"
#include <string>
#include <atomic>

class GameClient {
private:
    GameState gameState;
    Map map;
    Renderer renderer;
    InputHandler inputHandler;

    int localPlayerId;
    std::string playerName;
    std::string serverAddress;
    int serverPort;

    std::atomic<bool> running;
    float lastUpdateTime;

public:
    GameClient(const std::string& name, const std::string& server = "127.0.0.1", int port = 7777);
    ~GameClient();

    bool initialize();
    void run();
    void shutdown();

private:
    void processInput();
    void updateGame(float deltaTime);
    void render();

    bool connectToServer();
    void disconnectFromServer();
    void sendInputToServer(const InputState& input);
    void receiveGameStateFromServer();
};
