#pragma once

#include "Entity.h"

enum class ZombieType {
    NORMAL,
    FAST,
    HEAVY,
    BOSS
};

class Zombie : public Entity {
private:
    ZombieType zombieType;
    float moveSpeed;
    float attackDamage;
    float attackRange;
    float attackCooldown;
    float lastAttackTime;
    Vector2 targetPosition;
    int targetPlayerId;

public:
    Zombie(int id, Vector2 spawnPos, ZombieType type = ZombieType::NORMAL);

    void update(float deltaTime) override;
    void setTarget(const Vector2& target, int playerId = -1);
    bool canAttack(float currentTime) const;
    float getAttackDamage() const { return attackDamage; }
    float getAttackRange() const { return attackRange; }
    void performAttack(float currentTime);

    ZombieType getZombieType() const { return zombieType; }
    int getTargetPlayerId() const { return targetPlayerId; }

private:
    void initializeStats();
};
