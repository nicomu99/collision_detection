//
// Created by nico on 12/8/24.
//

#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP
#include <memory>
#include <bits/stl_vector.h>

class Circle;
class Tile;
class MoveResult;
struct GridEdge;
class Map;
struct Vector2d;
class Entity;
class Rectangle;

class CollisionHandler {
public:
    static void projectRectangleOntoAxis(const Rectangle* rect, const Vector2d& axis, double& min, double& max);
    static void computeAxes(const Rectangle* rect, std::vector<Vector2d>& edges);
    static bool intervalsOverlap(double min_a, double max_a, double min_b, double max_b);
    static double computeOverlap(double min_a, double max_a, double min_b, double max_b);

    static void handleCollision(const Rectangle* rect, const Rectangle* rectangle, MoveResult& move_result);
    static void handleCollision(const Rectangle* rect, const Circle* circle, MoveResult& move_result, bool determine_rectangle_velocity);
    static void handleCollision(const Circle* circle, const Circle* other_circle, Vector2d& velocity);

    static void checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result);
    static void checkEntityCollisions(Circle* rect, Entity* other_entity, MoveResult& move_result);

    static bool isWallCollision(const Rectangle* rect, const Map& map, GridEdge& hit_edge, Tile& collision_tile);
    static bool isWallCollision(const Circle* rect, const Map& map, GridEdge& hit_edge, Tile& collision_tile);

    static double computeWallCollisionPosition(const Rectangle* rect, const Tile& tile, GridEdge grid_edge, double delta_time);

    static void handleWallCollisions(const Rectangle* rect, const Map& map, MoveResult& move_result, double delta_time);
    static double computeWallCollisionPosition(const Circle* circle, const Tile& tile, GridEdge grid_edge, double delta_time);
    static void handleWallCollisions(const Circle* circle, const Map& map, MoveResult& move_result, double delta_time);

    static void checkCollisions(Entity* entity, const Map& map, const std::vector<std::unique_ptr<Entity>>& vector, double delta_time);
};

#endif //COLLISIONHANDLER_HPP
