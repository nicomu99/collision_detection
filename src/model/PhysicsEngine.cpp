//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

#include "CollisionHandler.hpp"
#include "Entity.hpp"
#include "GridEdge.hpp"
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

Vector2d PhysicsEngine::calculateTrajectory(Rectangle* rect, double delta_time) {
    return rect->getVelocity() * /* calculateDirection(rect->getRotation()) */ rect->getSpeed() * delta_time;
}

void PhysicsEngine::calculateMove(Rectangle* rect, const Map& map, double delta_time,
                                  const std::vector<std::unique_ptr<Entity> >& entities) {
    Vector2d trajectory;

    CollisionHandler::handleWallCollisions(rect, rect->getPositionAfterMove(trajectory), map);

    // Check for entity collision
    for (const auto& other_entity: entities) {
        if (other_entity.get() == rect) continue;

        CollisionHandler::isEntityCollision(rect, other_entity.get());
    }

    trajectory = calculateTrajectory(rect, delta_time);
    rect->move(trajectory);
}


void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity> >& entities, const Map& map,
                                       double delta_time) {
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            calculateMove(rect, map, delta_time, entities);
        }
    }
}
