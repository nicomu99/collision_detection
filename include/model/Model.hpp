//
// Created by nico on 11/29/24.
//

#ifndef MODEL_HPP
#define MODEL_HPP
#include <memory>
#include <vector>

#include "Entity.hpp"
#include "Map.hpp"
#include "PhysicsEngine.hpp"

class Model {
    Map map;
    std::vector<std::unique_ptr<Entity>> entities;
    PhysicsEngine physics_engine;
public:
    Model();

    [[nodiscard]] const std::vector<std::unique_ptr<Entity>>& getEntities() const;
    [[nodiscard]] const Map& getMap() const;

    void update(double delta_time);
};

#endif //MODEL_HPP
