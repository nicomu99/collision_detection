//
// Created by nico on 12/11/24.
//
#include "MoveResult.hpp"

MoveResult::MoveResult(): is_collision(false), updated_velocity(0, 0), new_position(0, 0) { }

MoveResult::MoveResult(Vector2d velocity, Vector2d position): is_collision(true), updated_velocity(velocity), new_position(position) { }

const Vector2d& MoveResult::getUpdatedVelocity() const {
    return updated_velocity;
}

const Vector2d& MoveResult::getNewPosition() const {
    return new_position;
}

void MoveResult::setUpdatedVelocity(Vector2d updated_velocity) {
    this->updated_velocity = updated_velocity;
}

void MoveResult::setNewPosition(Vector2d new_position) {
    this->new_position = new_position;
}

