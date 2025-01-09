//
// Created by nico on 11/29/24.
//
#include "Model.hpp"

#include "Rectangle.hpp"
#include "Circle.hpp"

Model::Model(): map(Map()), entities(std::vector<std::unique_ptr<Entity> >()), physics_engine(PhysicsEngine()) {
    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(500, 400), 0, Vector2d(1, 0), 1000, 100, 100, "Rotated Object")
    );

    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(1000, 450), 0, Vector2d(-1, 0), 1000, 100, 700, "Small Object")
    );

    /*entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(120, 700), 0, Vector2d(0, -1), 1000, 100, 500, "Negative Velocity Object")
    );

    entities.emplace_back(std::make_unique<Rectangle>(
        Vector2d(620, 500), 0, Vector2d(1, 0), 1000, 100, 100, "Large Object")
    );

    entities.emplace_back(std::make_unique<Circle>(
            Vector2d(700, 1300), 0, Vector2d(0, 1), 1000, 50.0, "Circle")
    );

    entities.emplace_back(std::make_unique<Circle>(
        Vector2d(600, 1000), 0, Vector2d(-0.3, -1), 1000, 50.0, "Circle")
    );*/
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
