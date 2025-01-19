//
// Created by nico on 1/19/25.
//
#include "CircleWallCollision.hpp"

#include <algorithm>

#include "Circle.hpp"
#include "Vector2d.hpp"
#include "Tile.hpp"
#include "Map.hpp"

namespace CircleWallCollision {
    void handleWallCollisions(const Circle* circle, const Map& map, MoveResult& move_result,
                              double delta_time) {
        Vector2d circle_position = circle->getPosition();
        Vector2d circle_velocity = circle->getVelocity();

        Vector2d wall_normal{0, 0};
        if (Tile collision_tile; isWallCollision(circle, map, wall_normal, collision_tile)) {
            double collision_time = computeWallCollisionPosition(circle, collision_tile, wall_normal, delta_time);
            circle_position = circle_position - circle_velocity * delta_time * collision_time;
            circle_velocity = circle_velocity - circle_velocity * wall_normal * wall_normal * 2;
        }

        move_result.setUpdatedVelocity(circle_velocity);
        move_result.setNewPosition(circle_position);
    }

    bool isWallCollision(const Circle* circle, const Map& map, Vector2d& wall_normal,
                         Tile& collision_tile) {
        double radius = circle->getRadius();
        Vector2d center = circle->getPosition();

        double left = center.x - radius;
        double right = center.x + radius;
        double top = center.y - radius;
        double bottom = center.y + radius;
        bool collision_detected = false;
        double max_collision_size = -0.1;
        for (int x = static_cast<int>(left); x <= right; x += Map::TILE_SIZE) {
            for (int y = static_cast<int>(top); y <= bottom; y += Map::TILE_SIZE) {
                const Tile& tile = map.getTile(x, y);
                double penetration_x = std::clamp(center.x, tile.getLeft(), tile.getRight());
                double penetration_y = std::clamp(center.y, tile.getTop(), tile.getBottom());

                if (tile.isWall() && center.euclidean(Vector2d(penetration_x, penetration_y)) < radius) {
                    collision_detected = true;

                    double overlap_x = radius - std::abs(center.x - penetration_x);
                    double overlap_y = radius - std::abs(center.y - penetration_y);

                    double collision_size = overlap_x * overlap_y;

                    if (collision_size > max_collision_size) {
                        max_collision_size = collision_size;
                        collision_tile = tile;
                        if (overlap_x < overlap_y) {
                            if (center.x < tile.getLeft()) {
                                wall_normal = {-1, 0};
                            } else {
                                wall_normal = {1, 0};
                            }
                        } else {
                            if (center.y > tile.getBottom()) {
                                wall_normal = {0, 1};
                            } else {
                                wall_normal = {0, -1};
                            }
                        }
                    } else if (collision_size == max_collision_size) {
                        if (overlap_x < overlap_y) {
                            if (center.x < tile.getLeft()) {
                                // Hit the left edge
                                wall_normal = {-1, 1};
                            } else {
                                // Hit the right edge
                                wall_normal = {1, 1};
                            }
                        } else {
                            if (center.y > tile.getBottom()) {
                                // Hit the bottom edge
                                wall_normal = {1, 1};
                            } else {
                                // Hit the top edge
                                wall_normal = {1, -1};
                            }
                        }
                    }
                }
            }
        }

        return collision_detected;
    }

    double computeWallCollisionPosition(const Circle* circle, const Tile& tile, Vector2d wall_normal,
                                     double delta_time) {
        Vector2d circle_pos = circle->getPosition();
        double radius = circle->getRadius();
        Vector2d circle_velocity = circle->getVelocity();
        double speed = circle->getSpeed();
        circle_velocity *= speed * delta_time;

        double collision_time = 0.0;
        if (wall_normal.x == 1 && wall_normal.y == 0) {
            collision_time = (tile.getRight() - radius - circle_pos.x) / circle_velocity.x;
        } else if (wall_normal.x == -1 && wall_normal.y == 0) {
            collision_time = (tile.getLeft() + radius - circle_pos.x) / circle_velocity.x;
        } else if (wall_normal.x == 0 && wall_normal.y == -1) {
            collision_time = (tile.getTop() + radius - circle_pos.y) / circle_velocity.y;
        } else if (wall_normal.x == 0 && wall_normal.y == 1) {
            collision_time = (tile.getBottom() - radius - circle_pos.y) / circle_velocity.y;
        } else {
            collision_time = 0.1;
        }

        return collision_time;
    }
}
