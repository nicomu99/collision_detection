//
// Created by nico on 12/8/24.
//
#include <CollisionHandler.hpp>

#include "Entity.hpp"

void CollisionHandler::checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    if (other_entity->isCollision(rect)) {
        other_entity->handleCollision(rect, move_result);
    }
}

void CollisionHandler::checkEntityCollisions(Circle* circle, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    if (other_entity->isCollision(circle)) {
        other_entity->handleCollision(circle, move_result);
    }
}

void CollisionHandler::resolveCollisions(Entity* entity, const Map& map,
                                       const std::vector<std::unique_ptr<Entity> >& entities,
                                       double delta_time) {
    MoveResult move_result(entity->getVelocity(), entity->getPosition());

    entity->handleWallCollisions(map, move_result, delta_time);
    for (const auto& other_entity: entities) {
        if (other_entity.get() == entity) continue;

        entity->checkEntityCollisions(other_entity.get(), move_result);
    }

    entity->setMoveResult(move_result);
}
