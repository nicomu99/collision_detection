//
// Created by nico on 11/29/24.
//
#include "Model.hpp"

#include "Rectangle.hpp"

Model::Model(): map(Map()), entities(std::vector<std::unique_ptr<Entity> >()), physics_engine(PhysicsEngine()) {
    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(500, 500), 0, Vector2d(-1, 0), 1000, 100, 400, "Large Object")
    );

    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(1200, 450), 0, Vector2d(1, 0), 1000, 100, 200, "Other Object")
    );

    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(1000, 500), 0, Vector2d(1, 0), 1000, 100, 100, "Small Object")
    );

    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(120, 700), 0, Vector2d(0, -1), 1000, 100, 500, "Negative Velocity Object")
    );
}

const std::vector<std::unique_ptr<Entity> >& Model::getEntities() const {
    return entities;
}

const Map& Model::getMap() const {
    return map;
}

void Model::update(double delta_time) {
    PhysicsEngine::manipulateEntities(entities, map, delta_time);
}
