//
// Created by nico on 11/29/24.
//


#include "Model.hpp"

#include "Rectangle.hpp"

Model::Model(): entities(std::vector<std::unique_ptr<Entity> >()) {
    entities.emplace_back(std::make_unique<Rectangle>(
            Vector2d(100, 100), 0, Vector2d(1, 0), 300.0, 50, 100)
    );
}

const std::vector<std::unique_ptr<Entity>>& Model::getEntities() const {
    return entities;
}
