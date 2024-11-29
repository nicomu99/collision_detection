//
// Created by nico on 11/30/24.
//
#include "Entity.hpp"

Entity::Entity(Vector2d position, int rotation, Vector2d velocity, double speed)
    : position(position), rotation(rotation), velocity(velocity), speed(speed) {
}

const Vector2d& Entity::getPosition() const {
    return position;
}
