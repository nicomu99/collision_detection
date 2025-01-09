//
// Created by nico on 11/30/24.
//

#ifndef ENTITY_HPP
#define ENTITY_HPP
#include <string>
#include <bits/unique_ptr.h>

#include "MoveResult.hpp"
#include "Vector2d.hpp"

class Circle;
class Rectangle;
class Map;

class Entity {
public:
    Vector2d current_position;
    Vector2d previous_position;
    Vector2d velocity;
    double speed;
    int rotation;
    std::string id;
    MoveResult move_result;
    double mass;

    Entity(Vector2d position, int rotation, Vector2d velocity, double speed, std::string id, double mass);
    virtual ~Entity() = default;

    [[nodiscard]] const Vector2d& getPosition() const;
    [[nodiscard]] const Vector2d& getPreviousPosition() const;
    [[nodiscard]] int getRotation() const;
    [[nodiscard]] double getSpeed() const;
    [[nodiscard]] const Vector2d& getVelocity() const;
    [[nodiscard]] Vector2d getInterpolatedPosition(double alpha) const;
    [[nodiscard]] Vector2d getPositionAfterMove(Vector2d vector2d) const;
    [[nodiscard]] double getMass() const;

    void setMoveResult(const MoveResult&);
    void setVelocity(Vector2d);
    void setCurrentPosition(Vector2d);

    virtual void move(Vector2d vector2d);
    virtual void update();
    virtual void revertMove();

    virtual void handleWallCollisions(const Map& map, MoveResult& move_result, double delta_time) = 0;
    virtual void checkEntityCollisions(Entity* other_entity, MoveResult& move_result) = 0;

    virtual bool isCollision(const Rectangle* rectangle) = 0;
    virtual bool isCollision(const Circle* circle) = 0;
};

#endif //ENTITY_HPP
