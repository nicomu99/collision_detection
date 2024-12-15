//
// Created by nico on 11/30/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>
#include <vector>

#include "MoveResult.hpp"
#include "Vector2d.hpp"

class Entity {
public:
    Vector2d current_position;
    Vector2d previous_position;
    Vector2d velocity;
    double speed;
    int rotation;
    std::string id;
    MoveResult move_result;

    Entity(Vector2d position, int rotation, Vector2d velocity, double speed, std::string id);
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2d& getPosition() const;
    [[nodiscard]] int getRotation() const;
    [[nodiscard]] double getSpeed() const;
    [[nodiscard]] const Vector2d& getVelocity() const;
    [[nodiscard]] Vector2d getInterpolatedPosition(double alpha) const;
    [[nodiscard]] Vector2d getPositionAfterMove(Vector2d vector2d) const;

    void setMoveResult(const MoveResult&);
    void setVelocity(Vector2d);
    void setCurrentPosition(Vector2d);

    virtual void move(Vector2d vector2d) = 0;
    virtual void update() = 0;
    virtual void revertMove() = 0;
    virtual void calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) = 0;
};

#endif //ENTITY_HPP
