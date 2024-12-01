//
// Created by nico on 11/30/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "Vector2d.hpp"

class Entity {
    Vector2d current_position;
    Vector2d previous_position;
    int rotation;
    Vector2d velocity;
    double speed;

public:
    Entity(Vector2d position, int rotation, Vector2d velocity, double speed);
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2d& getPosition() const;
    [[nodiscard]] int getRotation() const;
    [[nodiscard]] double getSpeed() const;
    [[nodiscard]] Vector2d getInterpolatedPosition(double alpha) const;

    void move(Vector2d vector2d);
};

#endif //ENTITY_HPP
