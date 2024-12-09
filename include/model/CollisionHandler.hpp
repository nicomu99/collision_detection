//
// Created by nico on 12/8/24.
//

#ifndef COLLISIONHANDLER_HPP
#define COLLISIONHANDLER_HPP

struct GridEdge;
class Map;
struct Vector2d;
class Entity;
class Rectangle;

class CollisionHandler {
public:
    static void handleRectangleCollision(Rectangle* rect, Rectangle* rectangle);
    static bool isEntityCollision(Rectangle* rect, Entity* other_entity);
    static bool checkRectangleCollision(const Rectangle* rect, const Rectangle* rectangle);

    static void handleWallCollisions(Rectangle* rect, Vector2d new_rectangle_position, const Map& map);
    static bool checkWallCollisions(Rectangle* rect, Vector2d new_rectangle_position, const Map& map, GridEdge& hit_edge);
};

#endif //COLLISIONHANDLER_HPP
