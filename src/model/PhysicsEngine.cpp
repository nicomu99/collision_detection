//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <iostream>
#include <cmath>

#include "Entity.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "SDLManager.hpp"
#include "Vector2d.hpp"

PhysicsEngine::PhysicsEngine() = default;

Vector2d PhysicsEngine::calculateDirection(int rotation) {
    const double radians = rotation * M_PI / 180;
    return {
        cos(radians),
        sin(radians)
    };
}

bool PhysicsEngine::isWallCollision(Rectangle* rect, Vector2d position, const Map& map) {
    std::vector<Vector2d> corner_points{};
    rect->calculateCornerPoints(corner_points, position);

    double bottom = std::numeric_limits<double>::lowest();
    double right = std::numeric_limits<double>::lowest();
    double top = std::numeric_limits<double>::max();
    double left = std::numeric_limits<double>::max();

    for(const Vector2d& corner: corner_points) {
        bottom = std::max(bottom, corner.y);
        right = std::max(right, corner.x);
        top = std::min(top, corner.y);
        left = std::min(left, corner.x);
    }

    for(int x = static_cast<int>(left); x <= right; x++) {
        for(int y = static_cast<int>(top); y <= bottom; y++) {
            if(map.isWallAt(x, y)) {
                return true;
            }
        }
    }

    return false;
}

void PhysicsEngine::calculateMove(Rectangle* rect, const Map& map, double delta_time) {
    Vector2d trajectory = rect->getVelocity() * /* calculateDirection(rect->getRotation()) */  rect->getSpeed() * delta_time;

    if(!isWallCollision(rect, rect->getPositionAfterMove(trajectory), map)) {
        rect->move(trajectory);
        return;
    }

    rect->setVelocity({rect->getVelocity().x * -1, 0});
}

void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity>>& entities, const Map& map, double delta_time) {
    for (auto& entity: entities) {
        if(auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            calculateMove(rect, map, delta_time);
        }
    }
}
