//
// Created by nico on 11/29/24.
//

#ifndef MODEL_HPP
#define MODEL_HPP
#include <memory>
#include <vector>

#include "Entity.hpp"

class Model {
    std::vector<std::unique_ptr<Entity>> entities;
public:
    Model();

    [[nodiscard]] const std::vector<std::unique_ptr<Entity>>& getEntities() const;
};

#endif //MODEL_HPP
