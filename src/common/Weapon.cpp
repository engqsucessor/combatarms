#include "Weapon.h"

Weapon::Weapon(WeaponType type, const std::string& name)
    : type(type)
    , name(name)
    , currentAmmo(30)
    , maxAmmo(30)
    , reserveAmmo(150)
    , damage(25.0f)
    , fireRate(0.1f)
    , reloadTime(2.0f)
    , lastShotTime(0.0f)
    , range(1000.0f)
    , isReloading(false)
    , reloadStartTime(0.0f)
{
    initializeStats();
}

void Weapon::initializeStats() {
    switch (type) {
        case WeaponType::ASSAULT_RIFLE:
            maxAmmo = 30;
            currentAmmo = 30;
            reserveAmmo = 180;
            damage = 25.0f;
            fireRate = 0.1f;
            reloadTime = 2.0f;
            range = 1000.0f;
            break;

        case WeaponType::SUBMACHINE_GUN:
            maxAmmo = 40;
            currentAmmo = 40;
            reserveAmmo = 200;
            damage = 18.0f;
            fireRate = 0.07f;
            reloadTime = 1.8f;
            range = 600.0f;
            break;

        case WeaponType::SHOTGUN:
            maxAmmo = 8;
            currentAmmo = 8;
            reserveAmmo = 32;
            damage = 60.0f;
            fireRate = 0.8f;
            reloadTime = 2.5f;
            range = 300.0f;
            break;

        case WeaponType::SNIPER_RIFLE:
            maxAmmo = 10;
            currentAmmo = 10;
            reserveAmmo = 40;
            damage = 100.0f;
            fireRate = 1.2f;
            reloadTime = 3.0f;
            range = 2000.0f;
            break;

        case WeaponType::PISTOL:
            maxAmmo = 15;
            currentAmmo = 15;
            reserveAmmo = 90;
            damage = 20.0f;
            fireRate = 0.3f;
            reloadTime = 1.5f;
            range = 500.0f;
            break;
    }
}

bool Weapon::canShoot(float currentTime) const {
    return !isReloading &&
           currentAmmo > 0 &&
           (currentTime - lastShotTime >= fireRate);
}

void Weapon::shoot(float currentTime) {
    if (!canShoot(currentTime)) return;

    currentAmmo--;
    lastShotTime = currentTime;

    // Auto-reload when empty
    if (currentAmmo == 0 && reserveAmmo > 0) {
        startReload(currentTime);
    }
}

void Weapon::startReload(float currentTime) {
    if (isReloading || currentAmmo == maxAmmo || reserveAmmo == 0) return;

    isReloading = true;
    reloadStartTime = currentTime;
}

void Weapon::updateReload(float currentTime) {
    if (!isReloading) return;

    if (currentTime - reloadStartTime >= reloadTime) {
        int ammoNeeded = maxAmmo - currentAmmo;
        int ammoToReload = (ammoNeeded <= reserveAmmo) ? ammoNeeded : reserveAmmo;

        currentAmmo += ammoToReload;
        reserveAmmo -= ammoToReload;

        isReloading = false;
    }
}
