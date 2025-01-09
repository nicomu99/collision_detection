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

    Vector2d& operator/(double divisor) {
        x /= divisor;
        y /= divisor;
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

    Vector2d& operator/=(double length) {
        this->x /= length;
        this->y /= length;
        return *this;
    }

    void to_normal() {
        double temp = x;
        x = -y;
        y = temp;
    }

    void normalize() {
        double length = this->length();
        if (length == 0.0) {
            length = 1.0;
        }
        x /= length;
        y /= length;
    }

    Vector2d operator*(const Vector2d& other) const {
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

    bool operator==(const Vector2d& other) const {
        return x == other.x && y == other.y;
    }

    [[nodiscard]] double euclidean(Vector2d other) const {
        return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
    }

    [[nodiscard]] double length() const {
        return sqrt(pow(x, 2) + pow(y, 2));
    }
};

#endif //VECTOR2D_HPP
