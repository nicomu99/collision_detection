//
// Created by nico on 12/15/24.
//

#ifndef CIRCLE_HPP
#define CIRCLE_HPP
#include "Entity.hpp"

class Circle : public Entity {
    double radius;

public:
    Circle();
    Circle(Vector2d position, int rotation, Vector2d velocity, double speed, double radius, std::string id);

    [[nodiscard]] double getRadius() const;

    void handleWallCollisions(const Map& map, MoveResult& move_result, double delta_time) override;
    void checkEntityCollisions(Entity* other_entity, MoveResult& move_result) override;

    bool isCollision(const Rectangle* rectangle) override;
    bool isCollision(const Circle* circle) override;
};

#endif //CIRCLE_HPP
