#include "Entity.h"

Entity::Entity(EntityType type, int id, Vector2 pos, float maxHp)
    : type(type)
    , id(id)
    , position(pos)
    , velocity(0, 0)
    , rotation(0.0f)
    , health(maxHp)
    , maxHealth(maxHp)
    , alive(true)
{
}

void Entity::update(float deltaTime) {
    if (!alive) return;

    // Apply velocity
    position += velocity * deltaTime;
}

void Entity::takeDamage(float damage) {
    if (!alive) return;

    health -= damage;
    if (health <= 0.0f) {
        health = 0.0f;
        alive = false;
    }
}
