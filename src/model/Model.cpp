//
// Created by nico on 11/29/24.
//
#include "Model.hpp"

#include "Rectangle.hpp"

Model::Model(): map(Map()), entities(std::vector<std::unique_ptr<Entity> >()), physics_engine(PhysicsEngine()) {
    /*entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(400, 400), 0, Vector2d(-1, 0), 1000, 100, 200, "Large Object")
    );*/

    /*entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(1200, 450), 0, Vector2d(1, 0), 1000, 100, 200)
    );*/

    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(600, 750), 90, Vector2d(1, 1), 1200, 100, 100, "Small Object")
    );

    /*entities.emplace_back(std::make_unique<Rectangle>(
        Vector2d(120, 120), 0, Vector2d(0, -1), 1200, 100, 100)
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
