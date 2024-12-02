//
// Created by nico on 11/30/24.
//
#include "Entity.hpp"

#include <iostream>

Entity::Entity(Vector2d position, int rotation, Vector2d velocity, double speed)
    : current_position(position), previous_position(position), rotation(rotation), velocity(velocity), speed(speed) {
}

const Vector2d& Entity::getPosition() const {
    return current_position;
}

int Entity::getRotation() const {
    return rotation;
}

double Entity::getSpeed() const {
    return speed;
}

Vector2d Entity::getInterpolatedPosition(double alpha) const {
    Vector2d output = previous_position * (1.0 - alpha) + current_position * alpha;
    return output;
}

Vector2d Entity::getPositionAfterMove(Vector2d target) const {
    return current_position + target;
}

void Entity::move(Vector2d target) {
    previous_position = current_position;
    current_position += target;
}
