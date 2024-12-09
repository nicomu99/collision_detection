//
// Created by nico on 11/30/24.
//
#include "Entity.hpp"

#include <iostream>
#include <utility>

Entity::Entity(Vector2d position, int rotation, Vector2d velocity, double speed, std::string id)
    : current_position(position), previous_position(position), velocity(velocity), speed(speed), rotation(rotation), id(std::move(id)) {
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

const Vector2d& Entity::getVelocity() const {
    return velocity;
}

Vector2d Entity::getInterpolatedPosition(double alpha) const {
    Vector2d output = previous_position * (1.0 - alpha) + current_position * alpha;
    return output;
}

Vector2d Entity::getPositionAfterMove(Vector2d target) const {
    return current_position + target;
}

void Entity::setVelocity(Vector2d new_velocity) {
    velocity = new_velocity;
}

void Entity::move(Vector2d target) {
    previous_position = current_position;
    current_position += target;
}
