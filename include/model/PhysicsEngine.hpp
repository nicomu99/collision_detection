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

class MoveResult;
struct GridEdge;
class Entity;

class PhysicsEngine {
public:
    PhysicsEngine();

    static Vector2d calculateTrajectory(const Entity* rect, double delta_time);

    static void performMove(Entity* rect, double delta_time);
    static void undoMove(Entity* entity);
    static void manipulateEntities(std::vector<std::unique_ptr<Entity> >&, const Map& map, double delta_time);
};

#endif //PHYSICSENGINE_HPP
