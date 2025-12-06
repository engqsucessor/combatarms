#include "Player.h"
#include <cmath>

Player::Player(int id, const std::string& name, Vector2 spawnPos, bool ai)
    : Entity(EntityType::PLAYER, id, spawnPos, 100.0f)
    , playerName(name)
    , moveSpeed(200.0f)
    , kills(0)
    , score(0)
    , isAI(ai)
{
    // Default weapon: Assault Rifle
    currentWeapon = std::make_unique<Weapon>(WeaponType::ASSAULT_RIFLE, "M4A1");
}

void Player::move(const Vector2& direction, float deltaTime) {
    if (!alive) return;

    Vector2 normalizedDir = direction.normalized();
    velocity = normalizedDir * moveSpeed;
}

void Player::aim(const Vector2& target) {
    if (!alive) return;

    Vector2 diff = target - position;
    rotation = std::atan2(diff.y, diff.x);
}

bool Player::shoot() {
    if (!alive || !currentWeapon) return false;

    float currentTime = 0.0f; // Will be provided by game state
    if (currentWeapon->canShoot(currentTime)) {
        currentWeapon->shoot(currentTime);
        return true;
    }
    return false;
}

void Player::reload() {
    if (!alive || !currentWeapon) return;

    float currentTime = 0.0f; // Will be provided by game state
    currentWeapon->startReload(currentTime);
}

void Player::equipWeapon(std::unique_ptr<Weapon> weapon) {
    currentWeapon = std::move(weapon);
}

void Player::update(float deltaTime) {
    Entity::update(deltaTime);

    if (currentWeapon) {
        float currentTime = 0.0f; // Will be provided by game state
        currentWeapon->updateReload(currentTime);
    }
}
