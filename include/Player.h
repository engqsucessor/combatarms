#pragma once

#include "Entity.h"
#include "Weapon.h"
#include <memory>
#include <string>

class Player : public Entity {
private:
    std::string playerName;
    std::unique_ptr<Weapon> currentWeapon;
    float moveSpeed;
    int kills;
    int score;
    bool isAI;

public:
    Player(int id, const std::string& name, Vector2 spawnPos, bool ai = false);

    void move(const Vector2& direction, float deltaTime);
    void aim(const Vector2& target);
    bool shoot();
    void reload();
    void equipWeapon(std::unique_ptr<Weapon> weapon);

    void update(float deltaTime) override;
    void addKill() { kills++; score += 100; }

    // Getters
    const std::string& getName() const { return playerName; }
    Weapon* getWeapon() const { return currentWeapon.get(); }
    int getKills() const { return kills; }
    int getScore() const { return score; }
    bool isAIControlled() const { return isAI; }
};
