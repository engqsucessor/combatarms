#include "Zombie.h"

Zombie::Zombie(int id, Vector2 spawnPos, ZombieType type)
    : Entity(EntityType::ZOMBIE, id, spawnPos, 100.0f)
    , zombieType(type)
    , moveSpeed(80.0f)
    , attackDamage(10.0f)
    , attackRange(30.0f)
    , attackCooldown(1.0f)
    , lastAttackTime(0.0f)
    , targetPosition(spawnPos)
    , targetPlayerId(-1)
{
    initializeStats();
}

void Zombie::initializeStats() {
    switch (zombieType) {
        case ZombieType::NORMAL:
            maxHealth = 100.0f;
            health = 100.0f;
            moveSpeed = 80.0f;
            attackDamage = 10.0f;
            break;

        case ZombieType::FAST:
            maxHealth = 60.0f;
            health = 60.0f;
            moveSpeed = 150.0f;
            attackDamage = 8.0f;
            attackCooldown = 0.7f;
            break;

        case ZombieType::HEAVY:
            maxHealth = 250.0f;
            health = 250.0f;
            moveSpeed = 50.0f;
            attackDamage = 20.0f;
            attackCooldown = 1.5f;
            break;

        case ZombieType::BOSS:
            maxHealth = 1000.0f;
            health = 1000.0f;
            moveSpeed = 60.0f;
            attackDamage = 30.0f;
            attackCooldown = 1.2f;
            break;
    }
}

void Zombie::update(float deltaTime) {
    if (!alive) return;

    // Move towards target
    Vector2 direction = targetPosition - position;
    float distance = direction.length();

    if (distance > attackRange) {
        Vector2 normalizedDir = direction.normalized();
        velocity = normalizedDir * moveSpeed;
    } else {
        velocity = Vector2(0, 0);
    }

    Entity::update(deltaTime);

    // Update rotation to face target
    if (distance > 0.1f) {
        rotation = std::atan2(direction.y, direction.x);
    }
}

void Zombie::setTarget(const Vector2& target, int playerId) {
    targetPosition = target;
    targetPlayerId = playerId;
}

bool Zombie::canAttack(float currentTime) const {
    return alive && (currentTime - lastAttackTime >= attackCooldown);
}

void Zombie::performAttack(float currentTime) {
    lastAttackTime = currentTime;
}
