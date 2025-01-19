//
// Created by nico on 1/19/25.
//

#ifndef RECTANGLEWALLCOLLISION_HPP
#define RECTANGLEWALLCOLLISION_HPP

class MoveResult;
class Tile;
struct Vector2d;
class Map;
class Rectangle;

namespace RectangleWallCollision {
    void handleWallCollisions(const Rectangle* rect, const Map& map, MoveResult& move_result, double delta_time);
    bool isWallCollision(const Rectangle* rect, const Map& map, Vector2d& collision_normal, Tile& collision_tile);
    double computeWallCollisionPosition(const Rectangle* rect, const Tile& tile, Vector2d collision_normal, double delta_time);
}

#endif //RECTANGLEWALLCOLLISION_HPP
