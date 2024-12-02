//
// Created by nico on 11/30/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <vector>

#include "Vector2d.hpp"

class Entity {
public:
    Vector2d current_position;
    Vector2d previous_position;
    Vector2d velocity;
    double speed;
    int rotation;

    Entity(Vector2d position, int rotation, Vector2d velocity, double speed);
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2d& getPosition() const;
    [[nodiscard]] int getRotation() const;
    [[nodiscard]] double getSpeed() const;
    [[nodiscard]] Vector2d getInterpolatedPosition(double alpha) const;
    [[nodiscard]] Vector2d getPositionAfterMove(Vector2d vector2d) const;

    virtual void move(Vector2d vector2d);
    virtual void calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) = 0;
};

#endif //ENTITY_HPP
