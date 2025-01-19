//
// Created by nico on 1/19/25.
//
#include "RectangleWallCollision.hpp"

#include "Vector2d.hpp"
#include "Rectangle.hpp"
#include "Map.hpp"

namespace RectangleWallCollision {
    void handleWallCollisions(const Rectangle* rect, const Map& map, MoveResult& move_result,
                              double delta_time) {
        Vector2d new_velocity = rect->getVelocity();
        Vector2d new_position = rect->getPosition();

        Vector2d collision_normal{0, 0};
        if (Tile collision_tile; isWallCollision(rect, map, collision_normal, collision_tile)) {
            new_velocity = new_velocity - (new_velocity * collision_normal) * collision_normal * 2;
            double collision_time = computeWallCollisionPosition(rect, collision_tile, collision_normal, delta_time);
            new_position = new_position - new_velocity * delta_time * collision_time;
        }

        move_result.setUpdatedVelocity(new_velocity);
        move_result.setNewPosition(new_position);
    }

    bool isWallCollision(const Rectangle* rect, const Map& map,
                         Vector2d& collision_normal, Tile& collision_tile) {
        Vector2d v_rect = rect->getVelocity();

        double top = rect->getTop();
        double bottom = rect->getBottom();
        double left = rect->getLeft();
        double right = rect->getRight();

        // Check for collision with the map
        double max_collision_size = -0.1;
        double max_overlap_x = 0.0;
        double max_overlap_y = 0.0;
        bool collision_detected = false;
        for (int x = static_cast<int>(left); x <= right; x += Map::TILE_SIZE) {
            for (int y = static_cast<int>(top); y <= bottom; y += Map::TILE_SIZE) {
                if (map.isWallAt(x, y)) {
                    const Tile& tile = map.getTile(x, y);

                    double overlap_x = std::min(rect->getRight(), tile.getRight()) - std::max(
                                           rect->getLeft(), tile.getLeft());
                    double overlap_y = std::min(rect->getBottom(), tile.getBottom()) - std::max(
                                           rect->getTop(), tile.getTop());

                    if (overlap_x < 0 || overlap_y < 0) {
                        continue;
                    }

                    double collision_size = overlap_x * overlap_y;
                    collision_detected = true;
                    if (collision_size > max_collision_size) {
                        collision_tile = tile;
                        max_collision_size = collision_size;
                        max_overlap_x = overlap_x;
                        max_overlap_y = overlap_y;
                        if (overlap_x < overlap_y) {
                            // Collision along x-axis
                            if (v_rect.x > 0) {
                                collision_normal = {-1, 0};
                            } else {
                                collision_normal = {1, 0};;
                            }
                        } else if (overlap_y < overlap_x) {
                            // Collision along y-axis
                            if (v_rect.y < 0) {
                                collision_normal = {0, 1};;
                            } else {
                                collision_normal = {0, -1};;
                            }
                        } else {
                            // Corner collision
                            if (v_rect.x >= 0 && v_rect.y >= 0) {
                                collision_normal = {-1, -1};
                            } else if (v_rect.x >= 0 && v_rect.y <= 0) {
                                collision_normal = {-1, 1};
                            } else if (v_rect.x <= 0 && v_rect.y >= 0) {
                                collision_normal = {1, -1};
                            } else {
                                collision_normal = {1, 1};
                            }
                        }
                    } else if (collision_size == max_collision_size) {
                        // Either corner hit or hit the same side of different tile
                        if (overlap_x == max_overlap_x && overlap_y == max_overlap_y) {
                            // Same side
                            continue;
                        }

                        // Corner hit
                        if (v_rect.x >= 0 && v_rect.y >= 0) {
                            collision_normal = {-1, -1};
                        } else if (v_rect.x >= 0 && v_rect.y <= 0) {
                            collision_normal = {-1, 1};
                        } else if (v_rect.x <= 0 && v_rect.y >= 0) {
                            collision_normal = {1, -1};
                        } else {
                            collision_normal = {1, 1};
                        }
                    }
                }
            }
        }

        return collision_detected;
    }

    double computeWallCollisionPosition(const Rectangle* rect, const Tile& tile,
                                        Vector2d collision_normal,
                                        double delta_time) {
        double collision_time = 0.0;
        Vector2d rect_velocity = rect->getVelocity();
        double speed = rect->getSpeed();
        rect_velocity *= speed;

        if (collision_normal.x == 1) {
            collision_time = (tile.getRight() - rect->getLeft()) / (delta_time * rect_velocity.x);
        } else if (collision_normal.x == -1) {
            collision_time = (tile.getLeft() - rect->getRight()) / (delta_time * rect_velocity.x);
        } else if (collision_normal.y == 1) {
            collision_time = (tile.getBottom() - rect->getTop()) / (delta_time * rect_velocity.y);
        } else if (collision_normal.y == -1) {
            collision_time = (tile.getTop() - rect->getBottom()) / (delta_time * rect_velocity.y);
        } else {
            collision_time = 0.1;
        }

        return collision_time;
    }
}
