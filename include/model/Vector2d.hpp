//
// Created by nico on 11/30/24.
//

#ifndef VECTOR2D_HPP
#define VECTOR2D_HPP

struct Vector2d {
    double x;
    double y;

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

    Vector2d operator+(const Vector2d& other) const {
        return {
            x + other.x,
            y + other.y
        };
    }
};

#endif //VECTOR2D_HPP
