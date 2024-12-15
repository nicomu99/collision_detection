//
// Created by nico on 12/8/24.
//

#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include <memory>
#include <bits/stl_vector.h>

class Tile;
class MoveResult;
struct GridEdge;
class Map;
struct Vector2d;
class Entity;
class Rectangle;

class CollisionHandler {
public:
    static void handleRectangleCollision(Rectangle* rect, Rectangle* rectangle);
    static bool checkEntityCollisions(Rectangle* rect, Entity* other_entity);
    static bool isCollision(const Rectangle* rect, const Rectangle* rectangle);

    static bool checkWallCollisions(Rectangle* rect, const Map& map, GridEdge& hit_edge, Tile& collision_tile);
    static double computeWallCollisionPosition(Rectangle* rect, const Tile& tile, GridEdge grid_edge, double delta_time);
    static void handleWallCollisions(Rectangle* rect, const Map& map, MoveResult& move_result, double delta_time);
    static void checkCollisions(Rectangle* rect, const Map& map, const std::vector<std::unique_ptr<Entity>>& vector, double delta_time);
};

#endif //COLLISIONHANDLER_HPP
