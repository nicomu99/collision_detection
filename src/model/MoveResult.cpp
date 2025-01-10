//
// Created by nico on 12/11/24.
//
#include "MoveResult.hpp"

MoveResult::MoveResult()
    : is_collision(false), old_position(0, 0), old_velocity(0, 0), new_position(0, 0), updated_velocity(0, 0) {
}

MoveResult::MoveResult(Vector2d velocity, Vector2d position)
    : is_collision(true), old_position(position), old_velocity(velocity), new_position(position), updated_velocity(velocity) {
}

const Vector2d& MoveResult::getUpdatedVelocity() const {
    return updated_velocity;
}

const Vector2d& MoveResult::getNewPosition() const {
    return new_position;
}

void MoveResult::setUpdatedVelocity(Vector2d updated_velocity) {
    // updated_velocity.normalize();
    if(this->updated_velocity == old_velocity) {
        this->updated_velocity = updated_velocity;
    }
}

void MoveResult::setNewPosition(Vector2d new_position) {
    if(this->new_position == old_position) {
        this->new_position = new_position;
    }
}
