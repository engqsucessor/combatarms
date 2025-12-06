#include "AIController.h"
#include <limits>
#include <cmath>

AIController::AIController(GameState* state, Map* gameMap)
    : gameState(state)
    , map(gameMap)
{
}

void AIController::updateAI(float deltaTime) {
    // Update all AI-controlled players
    for (const auto& pair : gameState->getPlayers()) {
        Player* player = pair.second.get();
        if (player->isAIControlled() && player->isAlive()) {
            updateAITeammate(player, deltaTime);
        }
    }

    // Update all zombies
    for (const auto& pair : gameState->getZombies()) {
        Zombie* zombie = pair.second.get();
        if (zombie->isAlive()) {
            updateZombieAI(zombie, deltaTime);
        }
    }
}

void AIController::updateAITeammate(Player* player, float deltaTime) {
    if (!player) return;

    // Find nearest zombie
    Zombie* nearestZombie = findNearestZombie(player->getPosition());

    if (nearestZombie) {
        Vector2 zombiePos = nearestZombie->getPosition();
        Vector2 playerPos = player->getPosition();

        // Aim at zombie
        player->aim(zombiePos);

        float distance = playerPos.distance(zombiePos);

        // Shoot if in range and has line of sight
        if (distance < 500.0f && hasLineOfSight(playerPos, zombiePos)) {
            player->shoot();
        }

        // Move behavior
        if (distance < 100.0f) {
            // Too close - back away
            Vector2 awayDir = (playerPos - zombiePos).normalized();
            player->move(awayDir, deltaTime);
        } else if (distance > 300.0f) {
            // Too far - move closer
            Vector2 towardDir = (zombiePos - playerPos).normalized();
            player->move(towardDir, deltaTime);
        } else {
            // Good distance - strafe
            Vector2 toZombie = (zombiePos - playerPos).normalized();
            Vector2 strafeDir(-toZombie.y, toZombie.x); // Perpendicular
            player->move(strafeDir, deltaTime);
        }
    } else {
        // No zombies - patrol or stay still
        player->move(Vector2(0, 0), deltaTime);
    }
}

void AIController::updateZombieAI(Zombie* zombie, float deltaTime) {
    if (!zombie) return;

    // Find nearest player
    Player* nearestPlayer = findNearestPlayer(zombie->getPosition());

    if (nearestPlayer && nearestPlayer->isAlive()) {
        Vector2 targetPos = nearestPlayer->getPosition();
        zombie->setTarget(targetPos, nearestPlayer->getId());

        // Check if close enough to attack
        float distance = zombie->getPosition().distance(targetPos);
        if (distance <= zombie->getAttackRange() && zombie->canAttack(gameState->getCurrentTime())) {
            zombie->performAttack(gameState->getCurrentTime());
            nearestPlayer->takeDamage(zombie->getAttackDamage());
        }
    }
}

Player* AIController::findNearestPlayer(const Vector2& position) {
    Player* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& pair : gameState->getPlayers()) {
        Player* player = pair.second.get();
        if (!player->isAlive()) continue;

        float distance = position.distance(player->getPosition());
        if (distance < minDistance) {
            minDistance = distance;
            nearest = player;
        }
    }

    return nearest;
}

Zombie* AIController::findNearestZombie(const Vector2& position) {
    Zombie* nearest = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (const auto& pair : gameState->getZombies()) {
        Zombie* zombie = pair.second.get();
        if (!zombie->isAlive()) continue;

        float distance = position.distance(zombie->getPosition());
        if (distance < minDistance) {
            minDistance = distance;
            nearest = zombie;
        }
    }

    return nearest;
}

Vector2 AIController::findCoverPosition(const Vector2& currentPos, const Vector2& threatPos) {
    // Simple cover finding - move perpendicular to threat
    Vector2 toThreat = (threatPos - currentPos).normalized();
    Vector2 perpendicular(-toThreat.y, toThreat.x);

    return currentPos + perpendicular * 100.0f;
}

bool AIController::hasLineOfSight(const Vector2& from, const Vector2& to) {
    // Simple line of sight check - could be improved with raycasting against walls
    Vector2 direction = to - from;
    float distance = direction.length();
    Vector2 normalized = direction.normalized();

    // Check points along the line
    for (float d = 0; d < distance; d += 10.0f) {
        Vector2 checkPoint = from + normalized * d;
        if (map->checkCollision(checkPoint, 5.0f)) {
            return false;
        }
    }

    return true;
}
