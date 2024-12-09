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
    static bool checkEntityCollisions(Rectangle* rect, Entity* other_entity);
    static bool checkRectangleCollision(const Rectangle* rect, const Rectangle* rectangle);

    static void handleWallCollisions(Rectangle* rect, const Map& map);
    static bool checkWallCollisions(Rectangle* rect, const Map& map, GridEdge& hit_edge);
};

#endif //COLLISIONHANDLER_HPP
