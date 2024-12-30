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

const Vector2d& Entity::getPreviousPosition() const {
    return previous_position;
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

void Entity::setMoveResult(const MoveResult& move_result) {
    this->move_result = move_result;
}

void Entity::setVelocity(Vector2d new_velocity) {
    velocity = new_velocity;
}

void Entity::setCurrentPosition(Vector2d current_position) {
    this->current_position = current_position;
}

void Entity::move(Vector2d target) {
    previous_position = current_position;
    current_position += target;
}

void Entity::update() {
    this->velocity = move_result.getUpdatedVelocity();
    this->current_position = move_result.getNewPosition();
}

void Entity::revertMove() {
    current_position = previous_position;
}
