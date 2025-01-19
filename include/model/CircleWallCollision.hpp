//
// Created by nico on 1/19/25.
//

#ifndef CIRCLEWALLCOLLISION_HPP
#define CIRCLEWALLCOLLISION_HPP


class Tile;
struct Vector2d;
class MoveResult;
class Map;
class Circle;

namespace CircleWallCollision {
    void handleWallCollisions(const Circle* circle, const Map& map, MoveResult& move_result, double delta_time);
    bool isWallCollision(const Circle* circle, const Map& map, Vector2d& wall_normal, Tile& collision_tile);
    double computeWallCollisionPosition(const Circle* circle, const Tile& tile, Vector2d wall_normal,
                                        double delta_time);
};

#endif //CIRCLEWALLCOLLISION_HPP
