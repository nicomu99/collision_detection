//
// Created by nico on 11/30/24.
//

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP
#include <cmath>

struct Vector2d {
    double x;
    double y;

    Vector2d(): x(0), y(0) { }

    Vector2d(double x, double y): x(x), y(y) { }

    Vector2d operator*(double scalar) const {
        return {
            x * scalar,
            y * scalar
        };
    }

    Vector2d& operator+=(Vector2d other) {
        x += other.x;
        y += other.y;
        return *this;
    }

    Vector2d& operator-=(const Vector2d& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }


    Vector2d operator+(const Vector2d& other) const {
        return {
            x + other.x,
            y + other.y
        };
    }

    Vector2d& operator*=(double x) {
        this->x *= x;
        this->y *= x;
        return *this;
    }

    Vector2d operator*(const Vector2d& other) const {
        // Define how multiplication is handled.
        // Example: element-wise multiplication
        return {this->x * other.x, this->y * other.y};
    }

    Vector2d operator-(const Vector2d& other) const {
        return {
            x - other.x,
            y - other.y
        };
    }

    [[nodiscard]] double dot(Vector2d other) const {
        return x * other.x + y * other.y;
    }

};

#endif //VECTOR2D_HPP
