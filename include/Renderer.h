#pragma once

#include "GameState.h"
#include "Map.h"
#include <SDL2/SDL.h>
#include <string>

class Renderer {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int screenWidth;
    int screenHeight;

    Vector2 cameraPos;
    float zoom;

public:
    Renderer(int width = 1280, int height = 720);
    ~Renderer();

    bool initialize();
    void shutdown();

    void clear();
    void present();

    void renderGame(const GameState& gameState, const Map& map, int localPlayerId);

    void setCamera(const Vector2& pos) { cameraPos = pos; }
    Vector2 worldToScreen(const Vector2& worldPos);

private:
    void renderMap(const Map& map);
    void renderPlayer(const Player* player, bool isLocal);
    void renderZombie(const Zombie* zombie);
    void renderHUD(const GameState& gameState, int localPlayerId);

    void drawRect(const Vector2& pos, float width, float height, SDL_Color color);
    void drawCircle(const Vector2& pos, float radius, SDL_Color color);
    void drawLine(const Vector2& start, const Vector2& end, SDL_Color color);
    void drawText(const std::string& text, int x, int y, SDL_Color color);
};
