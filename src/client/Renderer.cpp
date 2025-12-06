#include "Renderer.h"
#include <iostream>
#include <cmath>

Renderer::Renderer(int width, int height)
    : window(nullptr)
    , renderer(nullptr)
    , screenWidth(width)
    , screenHeight(height)
    , cameraPos(0, 0)
    , zoom(1.0f)
{
}

Renderer::~Renderer() {
    shutdown();
}

bool Renderer::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL initialization failed: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
        "Combat Arms: Cabin Fever",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "Window creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer creation failed: " << SDL_GetError() << std::endl;
        return false;
    }

    std::cout << "Renderer initialized successfully" << std::endl;
    return true;
}

void Renderer::shutdown() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }
    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
    SDL_Quit();
}

void Renderer::clear() {
    SDL_SetRenderDrawColor(renderer, 20, 25, 30, 255); // Dark background
    SDL_RenderClear(renderer);
}

void Renderer::present() {
    SDL_RenderPresent(renderer);
}

void Renderer::renderGame(const GameState& gameState, const Map& map, int localPlayerId) {
    // Update camera to follow local player
    const Player* localPlayer = const_cast<GameState&>(gameState).getPlayer(localPlayerId);
    if (localPlayer) {
        cameraPos = localPlayer->getPosition();
    }

    // Render map
    renderMap(map);

    // Render zombies
    for (const auto& pair : gameState.getZombies()) {
        const Zombie* zombie = pair.second.get();
        if (zombie->isAlive()) {
            renderZombie(zombie);
        }
    }

    // Render players
    for (const auto& pair : gameState.getPlayers()) {
        const Player* player = pair.second.get();
        if (player->isAlive()) {
            renderPlayer(player, player->getId() == localPlayerId);
        }
    }

    // Render HUD
    renderHUD(gameState, localPlayerId);
}

void Renderer::renderMap(const Map& map) {
    SDL_Color wallColor = {100, 100, 100, 255};

    // Render walls
    for (const auto& wall : map.getWalls()) {
        drawLine(wall.start, wall.end, wallColor);
    }

    // Render map bounds
    SDL_Color boundColor = {150, 150, 150, 255};
    drawRect(Vector2(map.getWidth()/2, map.getHeight()/2), map.getWidth(), map.getHeight(), boundColor);
}

void Renderer::renderPlayer(const Player* player, bool isLocal) {
    if (!player) return;

    SDL_Color playerColor = isLocal ? SDL_Color{0, 200, 0, 255} : SDL_Color{0, 100, 200, 255};

    Vector2 pos = player->getPosition();

    // Draw player circle
    drawCircle(pos, 20.0f, playerColor);

    // Draw direction indicator
    Vector2 dirEnd = pos + Vector2(std::cos(player->getRotation()), std::sin(player->getRotation())) * 30.0f;
    drawLine(pos, dirEnd, {255, 255, 255, 255});

    // Draw health bar
    float healthPercent = player->getHealth() / player->getMaxHealth();
    Vector2 screenPos = worldToScreen(pos);
    SDL_Rect healthBarBg = {(int)screenPos.x - 25, (int)screenPos.y - 35, 50, 5};
    SDL_Rect healthBarFg = {(int)screenPos.x - 25, (int)screenPos.y - 35, (int)(50 * healthPercent), 5};

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &healthBarBg);
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &healthBarFg);
}

void Renderer::renderZombie(const Zombie* zombie) {
    if (!zombie) return;

    SDL_Color zombieColor;
    switch (zombie->getZombieType()) {
        case ZombieType::NORMAL:
            zombieColor = {200, 0, 0, 255};
            break;
        case ZombieType::FAST:
            zombieColor = {255, 100, 0, 255};
            break;
        case ZombieType::HEAVY:
            zombieColor = {150, 0, 0, 255};
            break;
        case ZombieType::BOSS:
            zombieColor = {100, 0, 100, 255};
            break;
    }

    Vector2 pos = zombie->getPosition();
    float radius = (zombie->getZombieType() == ZombieType::HEAVY || zombie->getZombieType() == ZombieType::BOSS) ? 25.0f : 18.0f;

    drawCircle(pos, radius, zombieColor);

    // Draw health bar
    float healthPercent = zombie->getHealth() / zombie->getMaxHealth();
    Vector2 screenPos = worldToScreen(pos);
    SDL_Rect healthBarBg = {(int)screenPos.x - 25, (int)screenPos.y - 35, 50, 5};
    SDL_Rect healthBarFg = {(int)screenPos.x - 25, (int)screenPos.y - 35, (int)(50 * healthPercent), 5};

    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    SDL_RenderFillRect(renderer, &healthBarBg);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &healthBarFg);
}

void Renderer::renderHUD(const GameState& gameState, int localPlayerId) {
    const Player* player = const_cast<GameState&>(gameState).getPlayer(localPlayerId);
    if (!player) return;

    // Draw wave info
    std::string waveText = "Wave: " + std::to_string(gameState.getCurrentWave());
    drawText(waveText, 10, 10, {255, 255, 255, 255});

    // Draw zombie count
    std::string zombieText = "Zombies: " + std::to_string(gameState.getAliveZombieCount());
    drawText(zombieText, 10, 30, {255, 255, 255, 255});

    // Draw player health
    std::string healthText = "Health: " + std::to_string((int)player->getHealth()) + "/" + std::to_string((int)player->getMaxHealth());
    drawText(healthText, 10, 50, {255, 255, 255, 255});

    // Draw kills
    std::string killsText = "Kills: " + std::to_string(player->getKills());
    drawText(killsText, 10, 70, {255, 255, 255, 255});

    // Draw weapon info
    if (player->getWeapon()) {
        std::string ammoText = player->getWeapon()->getName() + ": " +
                              std::to_string(player->getWeapon()->getCurrentAmmo()) + "/" +
                              std::to_string(player->getWeapon()->getReserveAmmo());
        drawText(ammoText, screenWidth - 200, screenHeight - 30, {255, 255, 255, 255});
    }
}

Vector2 Renderer::worldToScreen(const Vector2& worldPos) {
    Vector2 relative = worldPos - cameraPos;
    return Vector2(
        screenWidth / 2 + relative.x * zoom,
        screenHeight / 2 + relative.y * zoom
    );
}

void Renderer::drawRect(const Vector2& pos, float width, float height, SDL_Color color) {
    Vector2 screenPos = worldToScreen(pos);
    SDL_Rect rect = {
        (int)(screenPos.x - width * zoom / 2),
        (int)(screenPos.y - height * zoom / 2),
        (int)(width * zoom),
        (int)(height * zoom)
    };

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}

void Renderer::drawCircle(const Vector2& pos, float radius, SDL_Color color) {
    Vector2 screenPos = worldToScreen(pos);
    int screenRadius = (int)(radius * zoom);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Simple circle drawing using midpoint circle algorithm
    for (int w = 0; w < screenRadius * 2; w++) {
        for (int h = 0; h < screenRadius * 2; h++) {
            int dx = screenRadius - w;
            int dy = screenRadius - h;
            if ((dx*dx + dy*dy) <= (screenRadius * screenRadius)) {
                SDL_RenderDrawPoint(renderer, screenPos.x + dx, screenPos.y + dy);
            }
        }
    }
}

void Renderer::drawLine(const Vector2& start, const Vector2& end, SDL_Color color) {
    Vector2 screenStart = worldToScreen(start);
    Vector2 screenEnd = worldToScreen(end);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(renderer, (int)screenStart.x, (int)screenStart.y, (int)screenEnd.x, (int)screenEnd.y);
}

void Renderer::drawText(const std::string& text, int x, int y, SDL_Color color) {
    // Simple text rendering - in a real game, use SDL_ttf
    // For now, just draw a placeholder
    SDL_Rect rect = {x, y, (int)text.length() * 8, 16};
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawRect(renderer, &rect);
}
