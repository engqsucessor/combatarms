#pragma once

#include <string>

enum class WeaponType {
    ASSAULT_RIFLE,
    SUBMACHINE_GUN,
    SHOTGUN,
    SNIPER_RIFLE,
    PISTOL
};

class Weapon {
private:
    WeaponType type;
    std::string name;
    int currentAmmo;
    int maxAmmo;
    int reserveAmmo;
    float damage;
    float fireRate;
    float reloadTime;
    float lastShotTime;
    float range;
    bool isReloading;
    float reloadStartTime;

public:
    Weapon(WeaponType type, const std::string& name);

    bool canShoot(float currentTime) const;
    void shoot(float currentTime);
    void startReload(float currentTime);
    void updateReload(float currentTime);

    // Getters
    WeaponType getType() const { return type; }
    const std::string& getName() const { return name; }
    int getCurrentAmmo() const { return currentAmmo; }
    int getMaxAmmo() const { return maxAmmo; }
    int getReserveAmmo() const { return reserveAmmo; }
    float getDamage() const { return damage; }
    float getRange() const { return range; }
    bool getIsReloading() const { return isReloading; }

    void addAmmo(int amount) { reserveAmmo += amount; }

private:
    void initializeStats();
};
