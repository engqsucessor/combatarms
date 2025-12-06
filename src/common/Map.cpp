#include "Map.h"
#include <cstdlib>
#include <ctime>

Map::Map()
    : width(1600.0f)
    , height(1200.0f)
{
    std::srand(std::time(nullptr));
}

void Map::loadCabinFeverMap() {
    walls.clear();
    spawnPoints.clear();

    // Cabin Fever is a small, compact map with a central cabin
    // Let's create a simple cabin layout

    // Outer boundaries
    addWall(Vector2(0, 0), Vector2(width, 0), 20.0f); // Top
    addWall(Vector2(0, height), Vector2(width, height), 20.0f); // Bottom
    addWall(Vector2(0, 0), Vector2(0, height), 20.0f); // Left
    addWall(Vector2(width, 0), Vector2(width, height), 20.0f); // Right

    // Central cabin structure (400x300)
    float cabinX = width / 2 - 200;
    float cabinY = height / 2 - 150;
    float cabinW = 400;
    float cabinH = 300;

    // Cabin walls with doorways
    addWall(Vector2(cabinX, cabinY), Vector2(cabinX + 150, cabinY), 15.0f); // Top left
    addWall(Vector2(cabinX + 250, cabinY), Vector2(cabinX + cabinW, cabinY), 15.0f); // Top right

    addWall(Vector2(cabinX, cabinY + cabinH), Vector2(cabinX + 150, cabinY + cabinH), 15.0f); // Bottom left
    addWall(Vector2(cabinX + 250, cabinY + cabinH), Vector2(cabinX + cabinW, cabinY + cabinH), 15.0f); // Bottom right

    addWall(Vector2(cabinX, cabinY), Vector2(cabinX, cabinY + 120), 15.0f); // Left top
    addWall(Vector2(cabinX, cabinY + 180), Vector2(cabinX, cabinY + cabinH), 15.0f); // Left bottom

    addWall(Vector2(cabinX + cabinW, cabinY), Vector2(cabinX + cabinW, cabinY + 120), 15.0f); // Right top
    addWall(Vector2(cabinX + cabinW, cabinY + 180), Vector2(cabinX + cabinW, cabinY + cabinH), 15.0f); // Right bottom

    // Interior obstacles (furniture)
    addWall(Vector2(cabinX + 50, cabinY + 50), Vector2(cabinX + 100, cabinY + 50), 10.0f);
    addWall(Vector2(cabinX + 300, cabinY + 200), Vector2(cabinX + 350, cabinY + 200), 10.0f);

    // Player spawn points (inside and around cabin)
    addPlayerSpawn(Vector2(cabinX + cabinW/2, cabinY + cabinH/2)); // Center of cabin
    addPlayerSpawn(Vector2(cabinX + 100, cabinY + 100));
    addPlayerSpawn(Vector2(cabinX + 300, cabinY + 200));
    addPlayerSpawn(Vector2(200, 200));

    // Zombie spawn points (around the map edges)
    for (int i = 0; i < 12; i++) {
        float angle = (i / 12.0f) * 2.0f * 3.14159f;
        float spawnDist = 500.0f;
        float x = width/2 + std::cos(angle) * spawnDist;
        float y = height/2 + std::sin(angle) * spawnDist;
        addZombieSpawn(Vector2(x, y));
    }
}

bool Map::checkCollision(const Vector2& position, float radius) const {
    // Check boundary collisions
    if (position.x - radius < 0 || position.x + radius > width ||
        position.y - radius < 0 || position.y + radius > height) {
        return true;
    }

    // Check wall collisions (simplified - point-to-line distance check)
    for (const auto& wall : walls) {
        Vector2 line = wall.end - wall.start;
        Vector2 toPoint = position - wall.start;

        float lineLength = line.length();
        if (lineLength == 0) continue;

        float t = toPoint.dot(line) / (lineLength * lineLength);
        t = (t < 0) ? 0 : (t > 1) ? 1 : t;

        Vector2 closestPoint = wall.start + line * t;
        float distance = position.distance(closestPoint);

        if (distance < radius + wall.thickness / 2) {
            return true;
        }
    }

    return false;
}

Vector2 Map::getRandomPlayerSpawn() const {
    if (spawnPoints.empty()) return Vector2(width/2, height/2);

    std::vector<Vector2> playerSpawns;
    for (const auto& spawn : spawnPoints) {
        if (spawn.isPlayerSpawn) {
            playerSpawns.push_back(spawn.position);
        }
    }

    if (playerSpawns.empty()) return Vector2(width/2, height/2);

    int idx = std::rand() % playerSpawns.size();
    return playerSpawns[idx];
}

Vector2 Map::getRandomZombieSpawn() const {
    if (spawnPoints.empty()) return Vector2(100, 100);

    std::vector<Vector2> zombieSpawns;
    for (const auto& spawn : spawnPoints) {
        if (!spawn.isPlayerSpawn) {
            zombieSpawns.push_back(spawn.position);
        }
    }

    if (zombieSpawns.empty()) return Vector2(100, 100);

    int idx = std::rand() % zombieSpawns.size();
    return zombieSpawns[idx];
}

void Map::addWall(Vector2 start, Vector2 end, float thickness) {
    walls.push_back({start, end, thickness});
}

void Map::addPlayerSpawn(Vector2 position) {
    spawnPoints.push_back({position, true});
}

void Map::addZombieSpawn(Vector2 position) {
    spawnPoints.push_back({position, false});
}
