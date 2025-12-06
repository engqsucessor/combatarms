#pragma once

#include <cmath>

class Vector2 {
public:
    float x, y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator+(const Vector2& other) const {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2& other) const {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(float scalar) const {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2& operator+=(const Vector2& other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    float length() const {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        float len = length();
        if (len > 0.0f) {
            return Vector2(x / len, y / len);
        }
        return Vector2(0, 0);
    }

    float dot(const Vector2& other) const {
        return x * other.x + y * other.y;
    }

    float distance(const Vector2& other) const {
        return (*this - other).length();
    }
};
