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

void PhysicsEngine::performMove(Rectangle* rect, double delta_time) {
    Vector2d trajectory = calculateTrajectory(rect, delta_time);
    rect->move(trajectory);
}

void PhysicsEngine::undoMove(Rectangle* rect) {
    rect->revertMove();
}

void PhysicsEngine::resolveMoveConsequences(Rectangle* rect, const Map& map,
                                            const std::vector<std::unique_ptr<Entity> >& entities) {
    CollisionHandler::handleWallCollisions(rect, map);

    // Check for entity collision
    for (const auto& other_entity: entities) {
        if (other_entity.get() == rect) continue;

        CollisionHandler::checkEntityCollisions(rect, other_entity.get());
    }
}


void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity> >& entities, const Map& map,
                                       double delta_time) {
    // First calculate the new velocity
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            performMove(rect, delta_time);
        }
    }

    // Then check move consequences
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            resolveMoveConsequences(rect, map, entities);
        }
    }

    // Overwrite move
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            undoMove(rect);
            performMove(rect, delta_time);
        }
    }
}
