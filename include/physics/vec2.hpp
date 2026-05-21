#pragma once

#include <cmath>

namespace physics {

struct Vec2 {
    double x = 0.0;
    double y = 0.0;

    constexpr Vec2() = default;
    constexpr Vec2(double x_value, double y_value) : x(x_value), y(y_value) {}

    constexpr Vec2 operator+(Vec2 rhs) const { return {x + rhs.x, y + rhs.y}; }
    constexpr Vec2 operator-(Vec2 rhs) const { return {x - rhs.x, y - rhs.y}; }
    constexpr Vec2 operator*(double scalar) const { return {x * scalar, y * scalar}; }
    constexpr Vec2 operator/(double scalar) const { return {x / scalar, y / scalar}; }

    constexpr Vec2& operator+=(Vec2 rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    constexpr Vec2& operator-=(Vec2 rhs) {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }

    constexpr Vec2& operator*=(double scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
};

constexpr Vec2 operator*(double scalar, Vec2 value) {
    return value * scalar;
}

constexpr double dot(Vec2 a, Vec2 b) {
    return a.x * b.x + a.y * b.y;
}

inline double length(Vec2 value) {
    return std::sqrt(dot(value, value));
}

} // namespace physics

