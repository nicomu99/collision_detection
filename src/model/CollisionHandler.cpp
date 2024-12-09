//
// Created by nico on 12/8/24.
//
#include <CollisionHandler.hpp>
#include <iostream>

#include "GridEdge.hpp"
#include "Map.hpp"
#include "Rectangle.hpp"
#include "Tile.hpp"

void CollisionHandler::handleRectangleCollision(Rectangle* rect, Rectangle* other_rect) {
    double overlap_x = std::min(rect->getRight(), other_rect->getRight()) - std::max(
                           rect->getLeft(), other_rect->getLeft());
    double overlap_y = std::min(rect->getBottom(), other_rect->getBottom()) - std::max(
                           rect->getTop(), other_rect->getTop());

    bool collide_on_x = overlap_x < overlap_y;
    Vector2d collision_normal;
    if (collide_on_x) {
        if (rect->getRight() < other_rect->getRight())
            collision_normal = Vector2d(-1, 0);
        else
            collision_normal = Vector2d(1, 0);
    } else {
        if (rect->getTop() < other_rect->getTop())
            collision_normal = Vector2d(0, -1);
        else
            collision_normal = Vector2d(0, 1);
    }

    if (collision_normal.x != 0 || collision_normal.y != 0) {
        std::cout << "=== " << rect->id << " ===" << std::endl;
        std::cout << "Position: " << rect->current_position.x << " " << rect->current_position.y << std::endl;
        std::cout << "Velocity: " << rect->velocity.x << " " << rect->velocity.y << std::endl;
        std::cout << collision_normal.x << " " << collision_normal.y << std::endl;
        std::cout << "Top: " << rect->getTop() << " Bottom: " << rect->getBottom() << std::endl;
        std::cout << "Left: " << rect->getLeft() << " Right: " << rect->getRight() << std::endl;
    }

    double normal_component = rect->getVelocity().dot(collision_normal);
    Vector2d new_velocity = rect->getVelocity() - collision_normal * normal_component * 2;
    rect->setVelocity(new_velocity);
}

bool CollisionHandler::checkEntityCollisions(Rectangle* rect, Entity* other_entity) {
    // First resolve what type of entity it is
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (checkRectangleCollision(rect, other_rect)) {
            handleRectangleCollision(rect, other_rect);
        }
    }

    return false;
}

bool CollisionHandler::checkRectangleCollision(const Rectangle* rect, const Rectangle* other_rect) {
    return rect->getRight() > other_rect->getLeft() && rect->getLeft() <= other_rect->getRight() &&
           rect->getBottom() > other_rect->getTop() && rect->getTop() <= other_rect->getBottom();
}

bool CollisionHandler::checkWallCollisions(Rectangle* rect, const Map& map,
                                           GridEdge& hit_edge) {
    double bottom = rect->getBottom();
    double right = rect->getRight();
    double top = rect->getTop();
    double left = rect->getLeft();

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

    return collision_detected;
}

void CollisionHandler::handleWallCollisions(Rectangle* rect, const Map& map) {
    GridEdge grid_edge = GridEdge::NONE;
    if (checkWallCollisions(rect, map, grid_edge)) {
        Vector2d old_velocity = rect->getVelocity();
        Vector2d wall_normal = grid_edge.toNormal();

        Vector2d new_velocity = old_velocity - (old_velocity * wall_normal) * wall_normal * 2;
        rect->setVelocity(new_velocity);
    }
}
