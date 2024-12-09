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

struct GridEdge;
class Entity;

class PhysicsEngine {
public:
    PhysicsEngine();

    static Vector2d calculateDirection(int rotation);
    static Vector2d calculateTrajectory(Rectangle* rect, double delta_time);

    static void resolveMoveConsequences(Rectangle* rect, const Map& map, const std::vector<std::unique_ptr<Entity>>& entities);
    static void performMove(Rectangle* rect, double delta_time);
    static void undoMove(Rectangle* entity);
    static void manipulateEntities(std::vector<std::unique_ptr<Entity>>&, const Map& map, double delta_time);
};

#endif //PHYSICSENGINE_HPP
