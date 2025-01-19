//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include "CollisionHandler.hpp"
#include "Entity.hpp"
#include "Map.hpp"
#include "Vector2d.hpp"

PhysicsEngine::PhysicsEngine() = default;

Vector2d PhysicsEngine::calculateTrajectory(const Entity* entity, double delta_time) {
    return entity->getVelocity() * entity->getSpeed() * delta_time;
}

void PhysicsEngine::performMove(Entity* entity, double delta_time) {
    Vector2d trajectory = calculateTrajectory(entity, delta_time);
    entity->move(trajectory);
}

void PhysicsEngine::undoMove(Entity* entity) {
    entity->revertMove();
}

void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity> >& entities, const Map& map,
                                       double delta_time) {
    // Perform hypothetical move
    for (auto& entity: entities) {
        performMove(entity.get(), delta_time);
    }

    // Then check move consequences
    for (auto& entity: entities) {
        CollisionHandler::resolveCollisions(entity.get(), map, entities, delta_time);
    }

    // Overwrite move
    for (auto& entity: entities) {
        undoMove(entity.get());
        entity->update();
    }
}
