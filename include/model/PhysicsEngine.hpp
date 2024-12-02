//
// Created by nico on 11/30/24.
//

#ifndef PHYSICSENGINE_HPP
#define PHYSICSENGINE_HPP
#include <memory>
#include <vector>

#include "Map.hpp"
#include "Rectangle.hpp"
#include "Vector2d.hpp"

class Entity;

class PhysicsEngine {
public:
    PhysicsEngine();
    static Vector2d calculateDirection(int rotation);
    static bool isWallCollision(Rectangle* rect, Vector2d position, const Map& map);

    static void calculateMove(Rectangle* rect, const Map& map, double delta_time);
    static void manipulateEntities(std::vector<std::unique_ptr<Entity>>&, const Map& map, double delta_time);
};

#endif //PHYSICSENGINE_HPP
