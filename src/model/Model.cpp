//
// Created by nico on 11/29/24.
//


#include "Model.hpp"

#include "Rectangle.hpp"

Model::Model(): entities(std::vector<std::unique_ptr<Entity> >()), physics_engine(PhysicsEngine()) {
    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(100, 100), 0, Vector2d(1, 0), 200, 100, 200)
    );
}

const std::vector<std::unique_ptr<Entity>>& Model::getEntities() const {
    return entities;
}

void Model::update(double delta_time) {
    PhysicsEngine::manipulateEntities(entities, delta_time);
}
