//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <iostream>
#include <cmath>

#include "Entity.hpp"
#include "Vector2d.hpp"

PhysicsEngine::PhysicsEngine() = default;

Vector2d PhysicsEngine::calculateDirection(int rotation) {
    const double radians = rotation * M_PI / 180;
    return {
        cos(radians),
        sin(radians)
    };
}

void PhysicsEngine::calculateMove(Entity* entity, double delta_time) {
    Vector2d trajectory = calculateDirection(entity->getRotation()) * entity->getSpeed() * delta_time;
    entity->move(trajectory);
}

void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity>>& entities, double delta_time) {
    for (auto& entity: entities) {
        calculateMove(entity.get(), delta_time);
    }
}
