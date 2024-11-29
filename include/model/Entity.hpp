//
// Created by nico on 11/30/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include "Vector2d.hpp"

class Entity {
    Vector2d position;
    int rotation;
    Vector2d velocity;
    double speed;

public:
    Entity(Vector2d position, int rotation, Vector2d velocity, double speed);
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2d& getPosition() const;
};

#endif //ENTITY_HPP
