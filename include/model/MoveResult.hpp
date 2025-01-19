//
// Created by nico on 12/11/24.
//

#ifndef MOVERESULT_HPP
#define MOVERESULT_HPP
#include "Vector2d.hpp"

class MoveResult {
    bool is_collision;
    Vector2d old_position;
    Vector2d old_velocity;
    Vector2d new_position;
    Vector2d updated_velocity;

public:
    MoveResult();
    MoveResult(Vector2d velocity, Vector2d position);

    [[nodiscard]] const Vector2d& getUpdatedVelocity() const;
    [[nodiscard]] Vector2d& getUpdatedVelocity();
    [[nodiscard]] const Vector2d& getNewPosition() const;

    void setUpdatedVelocity(Vector2d updated_velocity);
    void setNewPosition(Vector2d new_position);
};

#endif //MOVERESULT_HPP
