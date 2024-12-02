//
// Created by nico on 11/30/24.
//
#include "PhysicsEngine.hpp"

#include <iostream>
#include <cmath>

#include "Entity.hpp"
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

bool PhysicsEngine::isWallCollision(Rectangle* rect, Vector2d position, const Map& map, int &x, int &y, GridEdge &hit_edge) {
    // Calculate all corner points of the rectangle
    std::vector<Vector2d> corner_points{};
    rect->calculateCornerPoints(corner_points, position);

    double bottom = std::numeric_limits<double>::lowest();
    double right = std::numeric_limits<double>::lowest();
    double top = std::numeric_limits<double>::max();
    double left = std::numeric_limits<double>::max();

    // Determine the bounds of the rectangle
    for (const Vector2d& corner : corner_points) {
        bottom = std::max(bottom, corner.y);
        right = std::max(right, corner.x);
        top = std::min(top, corner.y);
        left = std::min(left, corner.x);
    }

    // Check for collision with the map
    for (x = static_cast<int>(left); x <= right; x++) {
        for (y = static_cast<int>(top); y <= bottom; y++) {
            if (map.isWallAt(x, y)) {
                // Check which corner is inside the wall
                for (const Vector2d& corner : corner_points) {
                    int corner_x = static_cast<int>(corner.x);
                    int corner_y = static_cast<int>(corner.y);

                    if (corner_x == x && corner_y == y) {

                        // Determine which edge of the grid cell is hit
                        double distance_to_top = std::abs(corner.y - (y + 1));
                        double distance_to_bottom = std::abs(corner.y - y);
                        double distance_to_left = std::abs(corner.x - x);
                        double distance_to_right = std::abs(corner.x - (x + 1));

                        // Find the smallest distance
                        double min_distance = std::ranges::min({distance_to_top, distance_to_bottom, distance_to_left, distance_to_right});

                        if(rect->velocity.x == 0 && rect->velocity.y > 0) {
                            hit_edge = GridEdge::TOP;
                        } else if (rect->velocity.x == 0 && rect->velocity.y < 0) {
                            hit_edge = GridEdge::BOTTOM;
                        } else if(rect->velocity.x > 0 && rect->velocity.y == 0) {
                            hit_edge = GridEdge::LEFT;
                        } else if(rect->velocity.x < 0 && rect->velocity.y == 0) {
                            hit_edge = GridEdge::RIGHT;
                        } else if (min_distance == distance_to_top) {
                            hit_edge = GridEdge::TOP;
                        } else if (min_distance == distance_to_bottom) {
                            hit_edge = GridEdge::BOTTOM;
                        } else if (min_distance == distance_to_left) {
                            hit_edge = GridEdge::LEFT;
                        } else if (min_distance == distance_to_right) {
                            hit_edge = GridEdge::RIGHT;
                        } else {
                            hit_edge = GridEdge::NONE;
                        }

                        return true;
                    }
                }
            }
        }
    }

    hit_edge = GridEdge::NONE;
    return false;
}


Vector2d PhysicsEngine::getWallNormal(GridEdge grid_edge) {
    switch(grid_edge) {
        case GridEdge::TOP:
            return {0, -1};
        case GridEdge::BOTTOM:
            return {0, 1};
        case GridEdge::LEFT:
            return {-1, 0};
        case GridEdge::RIGHT:
            return {1, 0};
        case GridEdge::NONE:
        default:
            return {0, 0};
    }
}

Vector2d PhysicsEngine::calculateTrajectory(Rectangle* rect, double delta_time) {
    return rect->getVelocity() * /* calculateDirection(rect->getRotation()) */  rect->getSpeed() * delta_time;
}

void PhysicsEngine::calculateMove(Rectangle* rect, const Map& map, double delta_time) {
    Vector2d trajectory = calculateTrajectory(rect, delta_time);

    int x, y = 0;
    GridEdge grid_edge;
    if(isWallCollision(rect, rect->getPositionAfterMove(trajectory), map, x, y, grid_edge)) {
        Vector2d old_velocity = rect->getVelocity();
        Vector2d wall_normal = getWallNormal(grid_edge);

        Vector2d new_velocity = old_velocity - (old_velocity * wall_normal) * wall_normal * 2;
        rect->setVelocity(new_velocity);
    }
    rect->move(trajectory);
}

void PhysicsEngine::manipulateEntities(std::vector<std::unique_ptr<Entity>>& entities, const Map& map, double delta_time) {
    for (auto& entity: entities) {
        if(auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            calculateMove(rect, map, delta_time);
        }
    }
}
