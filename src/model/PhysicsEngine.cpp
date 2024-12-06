//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <algorithm>
#include <iostream>
#include <cmath>

#include "Entity.hpp"
#include "GridEdge.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "SDLManager.hpp"
#include "Vector2d.hpp"

PhysicsEngine::PhysicsEngine() = default;

Vector2d PhysicsEngine::calculateDirection(int rotation) {
    const double radians = rotation * M_PI / 180;
    return {
        cos(radians),
        sin(radians)
    };
}

bool PhysicsEngine::isWallCollision(Rectangle* rect, Vector2d position, const Map& map, GridEdge& hit_edge) {
    // Calculate all corner points of the rectangle
    std::vector<Vector2d> corner_points{};
    rect->calculateCornerPoints(corner_points, position);

    double bottom = std::numeric_limits<double>::lowest();
    double right = std::numeric_limits<double>::lowest();
    double top = std::numeric_limits<double>::max();
    double left = std::numeric_limits<double>::max();

    // Determine the bounds of the rectangle
    for (const Vector2d& corner: corner_points) {
        bottom = std::max(bottom, corner.y);
        right = std::max(right, corner.x);
        top = std::min(top, corner.y);
        left = std::min(left, corner.x);
    }

    // Check for collision with the map
    double max_collision_size = 0.0;
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
                    max_collision_size = collision_size;
                    if (overlap_x < overlap_y) {
                        // Collision along x-axis
                        if (rect->getVelocity().x > 0) {
                            hit_edge = GridEdge::LEFT;
                        } else {
                            hit_edge = GridEdge::RIGHT;
                        }
                    } else {
                        if (rect->getVelocity().y < 0) {
                            hit_edge = GridEdge::TOP;
                        } else {
                            hit_edge = GridEdge::BOTTOM;
                        }
                    }
                }
            }
        }
    }

    if(collision_detected) {
        return true;
    }

    hit_edge = GridEdge::NONE;
    return false;
}

Vector2d PhysicsEngine::calculateTrajectory(Rectangle* rect, double delta_time) {
    return rect->getVelocity() * /* calculateDirection(rect->getRotation()) */ rect->getSpeed() * delta_time;
}

void PhysicsEngine::calculateMove(Rectangle* rect, const Map& map, double delta_time) {
    Vector2d trajectory = calculateTrajectory(rect, delta_time);

    GridEdge grid_edge(GridEdge::NONE);
    if (isWallCollision(rect, rect->getPositionAfterMove(trajectory), map, grid_edge)) {
        Vector2d old_velocity = rect->getVelocity();
        Vector2d wall_normal = grid_edge.toNormal();

        Vector2d new_velocity = old_velocity - (old_velocity * wall_normal) * wall_normal * 2;
        rect->setVelocity(new_velocity);
        trajectory = calculateTrajectory(rect, delta_time);
    }

    rect->move(trajectory);
}


void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity> >& entities, const Map& map,
                                       double delta_time) {
    for (auto& entity: entities) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            calculateMove(rect, map, delta_time);
        }
    }
}
