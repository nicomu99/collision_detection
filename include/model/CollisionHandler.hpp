//
// Created by nico on 12/8/24.
//

#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include <memory>
#include <vector>

class Map;
class Circle;
class MoveResult;
class Entity;
class Rectangle;

class CollisionHandler {
public:
    static void checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result);
    static void checkEntityCollisions(Circle* rect, Entity* other_entity, MoveResult& move_result);

    static void resolveCollisions(Entity* entity, const Map& map, const std::vector<std::unique_ptr<Entity>>& vector, double delta_time);
};

#endif //COLLISIONHANDLER_HPP
