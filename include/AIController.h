#pragma once

#include "GameState.h"
#include "Map.h"

class AIController {
private:
    GameState* gameState;
    Map* map;

public:
    AIController(GameState* state, Map* gameMap);

    void updateAI(float deltaTime);

private:
    void updateAITeammate(Player* player, float deltaTime);
    void updateZombieAI(Zombie* zombie, float deltaTime);

    Player* findNearestPlayer(const Vector2& position);
    Zombie* findNearestZombie(const Vector2& position);
    Vector2 findCoverPosition(const Vector2& currentPos, const Vector2& threatPos);
    bool hasLineOfSight(const Vector2& from, const Vector2& to);
};
