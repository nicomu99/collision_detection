//
// Created by nico on 11/30/24.
//

#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include <memory>
#include <vector>

#include "Rectangle.hpp"
#include "Vector2d.hpp"

class Entity;

class PhysicsEngine {
public:
    PhysicsEngine();
    static Vector2d calculateDirection(int rotation);

    static void calculateMove(Rectangle* entity, double delta_time);
    static void manipulateEntities(std::vector<std::unique_ptr<Entity>>&, double delta_time);
};

#endif //PHYSICSENGINE_HPP
