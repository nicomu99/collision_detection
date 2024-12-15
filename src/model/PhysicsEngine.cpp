//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>
#include <unordered_map>

#include "CollisionHandler.hpp"
#include "Entity.hpp"
#include "GridEdge.hpp"
#include "Map.hpp"
#include "MoveResult.hpp"
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
                                            const std::vector<std::unique_ptr<Entity> >& entities, double delta_time) {

}


void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity> >& entities, const Map& map,
                                       double delta_time) {
    // Perform hypothetical move
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            performMove(rect, delta_time);
        }
    }

    // Then check move consequences
    // For each entity object, create a movement result
    // That is used to update the object
    for (auto& entity: entities) {
        CollisionHandler::checkCollisions(entity.get(), map, entities, delta_time);
    }

    // Overwrite move
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            undoMove(rect);
            // performMove(rect, delta_time);
            rect->update();
        }
    }
}
