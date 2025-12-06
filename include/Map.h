#pragma once

#include "Vector2.h"
#include <vector>

struct Wall {
    Vector2 start;
    Vector2 end;
    float thickness;
};

struct SpawnPoint {
    Vector2 position;
    bool isPlayerSpawn;
};

class Map {
private:
    std::vector<Wall> walls;
    std::vector<SpawnPoint> spawnPoints;
    float width;
    float height;

public:
    Map();

    void loadCabinFeverMap();
    bool checkCollision(const Vector2& position, float radius) const;
    Vector2 getRandomPlayerSpawn() const;
    Vector2 getRandomZombieSpawn() const;

    const std::vector<Wall>& getWalls() const { return walls; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }

private:
    void addWall(Vector2 start, Vector2 end, float thickness = 10.0f);
    void addPlayerSpawn(Vector2 position);
    void addZombieSpawn(Vector2 position);
};
