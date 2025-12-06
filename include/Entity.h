#pragma once

#include "Vector2.h"
#include <string>

enum class EntityType {
    PLAYER,
    ZOMBIE,
    AI_TEAMMATE,
    PROJECTILE
};

class Entity {
protected:
    Vector2 position;
    Vector2 velocity;
    float rotation;
    float health;
    float maxHealth;
    bool alive;
    EntityType type;
    int id;

public:
    Entity(EntityType type, int id, Vector2 pos, float maxHp);
    virtual ~Entity() = default;

    virtual void update(float deltaTime);
    virtual void takeDamage(float damage);

    // Getters
    Vector2 getPosition() const { return position; }
    Vector2 getVelocity() const { return velocity; }
    float getRotation() const { return rotation; }
    float getHealth() const { return health; }
    float getMaxHealth() const { return maxHealth; }
    bool isAlive() const { return alive; }
    EntityType getType() const { return type; }
    int getId() const { return id; }

    // Setters
    void setPosition(const Vector2& pos) { position = pos; }
    void setVelocity(const Vector2& vel) { velocity = vel; }
    void setRotation(float rot) { rotation = rot; }
};
