#include "GameClient.h"
#include <iostream>
#include <chrono>
#include <thread>

GameClient::GameClient(const std::string& name, const std::string& server, int port)
    : playerName(name)
    , serverAddress(server)
    , serverPort(port)
    , localPlayerId(-1)
    , running(false)
    , lastUpdateTime(0.0f)
{
}

GameClient::~GameClient() {
    shutdown();
}

bool GameClient::initialize() {
    std::cout << "Initializing client..." << std::endl;

    if (!renderer.initialize()) {
        std::cerr << "Failed to initialize renderer!" << std::endl;
        return false;
    }

    // Load map
    map.loadCabinFeverMap();

    // For offline mode, create a local player
    Vector2 spawnPos = map.getRandomPlayerSpawn();
    localPlayerId = gameState.addPlayer(playerName, false);

    // Add AI teammates
    for (int i = 0; i < 3; i++) {
        std::string aiName = "AI_Teammate_" + std::to_string(i + 1);
        gameState.addPlayer(aiName, true);
    }

    // Start first wave
    gameState.startWave();

    std::cout << "Client initialized successfully" << std::endl;
    return true;
}

void GameClient::run() {
    running = true;
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (running) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastTime;
        float deltaTime = elapsed.count();
        lastTime = currentTime;

        processInput();
        updateGame(deltaTime);
        render();

        // Cap at ~60 FPS
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void GameClient::shutdown() {
    renderer.shutdown();
    std::cout << "Client shut down" << std::endl;
}

void GameClient::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
    }

    inputHandler.update();
    InputState input = inputHandler.getInputState(renderer.worldToScreen(Vector2(0, 0)));

    if (input.quit) {
        running = false;
    }

    // Apply local player input
    Player* player = gameState.getPlayer(localPlayerId);
    if (player && player->isAlive()) {
        player->move(input.moveDirection, 0.016f);
        player->aim(input.aimPosition);

        if (input.shoot) {
            if (player->shoot()) {
                // Check if we hit any zombies
                Vector2 playerPos = player->getPosition();
                Vector2 aimDir = (input.aimPosition - playerPos).normalized();
                float range = player->getWeapon() ? player->getWeapon()->getRange() : 1000.0f;

                // Simple raycast to find hit zombie
                for (const auto& pair : gameState.getZombies()) {
                    Zombie* zombie = pair.second.get();
                    if (!zombie->isAlive()) continue;

                    Vector2 toZombie = zombie->getPosition() - playerPos;
                    float distance = toZombie.length();

                    if (distance <= range) {
                        Vector2 dirToZombie = toZombie.normalized();
                        float dotProduct = aimDir.dot(dirToZombie);

                        // Check if zombie is in aim direction (within ~20 degrees)
                        if (dotProduct > 0.94f) {
                            float damage = player->getWeapon() ? player->getWeapon()->getDamage() : 25.0f;
                            zombie->takeDamage(damage);

                            if (!zombie->isAlive()) {
                                player->addKill();
                            }
                            break; // Hit first zombie in line
                        }
                    }
                }
            }
        }

        if (input.reload) {
            player->reload();
        }
    }
}

void GameClient::updateGame(float deltaTime) {
    gameState.update(deltaTime);

    // Simple AI for teammates and zombies (in offline mode)
    // In online mode, server would handle this

    // Update AI teammates
    for (const auto& pair : gameState.getPlayers()) {
        Player* player = pair.second.get();
        if (!player->isAIControlled() || !player->isAlive()) continue;

        // Find nearest zombie
        Zombie* nearestZombie = nullptr;
        float minDist = 999999.0f;

        for (const auto& zPair : gameState.getZombies()) {
            Zombie* z = zPair.second.get();
            if (!z->isAlive()) continue;

            float dist = player->getPosition().distance(z->getPosition());
            if (dist < minDist) {
                minDist = dist;
                nearestZombie = z;
            }
        }

        if (nearestZombie) {
            Vector2 zombiePos = nearestZombie->getPosition();
            player->aim(zombiePos);

            if (minDist < 100.0f) {
                Vector2 awayDir = (player->getPosition() - zombiePos).normalized();
                player->move(awayDir, deltaTime);
            } else if (minDist > 300.0f) {
                Vector2 towardDir = (zombiePos - player->getPosition()).normalized();
                player->move(towardDir, deltaTime);
            }

            if (player->shoot() && minDist < 500.0f) {
                nearestZombie->takeDamage(player->getWeapon() ? player->getWeapon()->getDamage() : 25.0f);
                if (!nearestZombie->isAlive()) {
                    player->addKill();
                }
            }
        }
    }

    // Update zombies
    for (const auto& pair : gameState.getZombies()) {
        Zombie* zombie = pair.second.get();
        if (!zombie->isAlive()) continue;

        // Find nearest player
        Player* nearestPlayer = nullptr;
        float minDist = 999999.0f;

        for (const auto& pPair : gameState.getPlayers()) {
            Player* p = pPair.second.get();
            if (!p->isAlive()) continue;

            float dist = zombie->getPosition().distance(p->getPosition());
            if (dist < minDist) {
                minDist = dist;
                nearestPlayer = p;
            }
        }

        if (nearestPlayer) {
            zombie->setTarget(nearestPlayer->getPosition(), nearestPlayer->getId());

            if (minDist <= zombie->getAttackRange() && zombie->canAttack(gameState.getCurrentTime())) {
                zombie->performAttack(gameState.getCurrentTime());
                nearestPlayer->takeDamage(zombie->getAttackDamage());
            }
        }
    }

    // Spawn zombies periodically
    static float spawnTimer = 0.0f;
    spawnTimer += deltaTime;

    if (gameState.getPhase() == GamePhase::WAVE_IN_PROGRESS && spawnTimer >= 2.0f) {
        if (gameState.getAliveZombieCount() < 20) {
            Vector2 spawnPos = map.getRandomZombieSpawn();
            gameState.addZombie(spawnPos, ZombieType::NORMAL);
            spawnTimer = 0.0f;
        }
    }

    // Check wave completion
    if (gameState.getPhase() == GamePhase::WAVE_COMPLETE) {
        static float waveCompleteTimer = 0.0f;
        waveCompleteTimer += deltaTime;

        if (waveCompleteTimer >= 5.0f) {
            gameState.startWave();
            waveCompleteTimer = 0.0f;
        }
    }

    // Check game over
    if (gameState.getPhase() == GamePhase::GAME_OVER) {
        std::cout << "Game Over! You survived " << gameState.getCurrentWave() << " waves!" << std::endl;
        running = false;
    }
}

void GameClient::render() {
    renderer.clear();
    renderer.renderGame(gameState, map, localPlayerId);
    renderer.present();
}

bool GameClient::connectToServer() {
    // TODO: Implement network connection
    return false;
}

void GameClient::disconnectFromServer() {
    // TODO: Implement disconnection
}

void GameClient::sendInputToServer(const InputState& input) {
    // TODO: Serialize and send input
}

void GameClient::receiveGameStateFromServer() {
    // TODO: Receive and deserialize game state
}
