//
// Created by nico on 12/8/24.
//
#include <CollisionHandler.hpp>
#include <iostream>

#include "GridEdge.hpp"
#include "Map.hpp"
#include "MoveResult.hpp"
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
        if (isCollision(rect, other_rect)) {
            handleRectangleCollision(rect, other_rect);
        }
    }

    return false;
}

bool CollisionHandler::isCollision(const Rectangle* rect, const Rectangle* other_rect) {
    return rect->getRight() > other_rect->getLeft() && rect->getLeft() <= other_rect->getRight() &&
           rect->getBottom() > other_rect->getTop() && rect->getTop() <= other_rect->getBottom();
}

bool CollisionHandler::checkWallCollisions(Rectangle* rect, const Map& map,
                                           GridEdge& hit_edge, Tile& collision_tile) {
    double top = rect->getTop();
    double bottom = rect->getBottom();
    double left = rect->getLeft();
    double right = rect->getRight();

    // Check for collision with the map
    double max_collision_size = -0.1;
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
                    if (overlap_x < overlap_y) {
                        // Collision along x-axis
                        if (rect->getVelocity().x > 0) {
                            hit_edge = GridEdge::LEFT;
                        } else {
                            hit_edge = GridEdge::RIGHT;
                        }
                    } else {
                        if (rect->getVelocity().y < 0) {
                            hit_edge = GridEdge::BOTTOM;
                        } else {
                            hit_edge = GridEdge::TOP;
                        }
                    }
                }
            }
        }
    }

    return collision_detected;
}

double CollisionHandler::computeWallCollisionPosition(Rectangle* rect, const Tile& tile, GridEdge grid_edge,
                                                      double delta_time) {
    double collision_time = 0.0;
    Vector2d rect_velocity = rect->getVelocity();
    double speed = rect->getSpeed();
    rect_velocity *= speed;
    switch (grid_edge.value) {
        case GridEdge::LEFT:
            if (rect_velocity.x > 0)
                collision_time = (tile.getLeft() - rect->getRight()) / (delta_time * rect_velocity.x);
            break;
        case GridEdge::RIGHT:
            if (rect_velocity.x < 0)
                collision_time = (tile.getRight() - rect->getLeft()) / (delta_time * rect_velocity.x);
            break;
        case GridEdge::BOTTOM:
            if (rect_velocity.y < 0)
                collision_time = (tile.getTop() - rect->getBottom()) / (delta_time * rect_velocity.y);
            break;
        case GridEdge::TOP:
            if (rect_velocity.y > 0)
                collision_time = (tile.getTop() - rect->getBottom()) / (delta_time * rect_velocity.y);
            break;
        default:
            break;
    }

    return collision_time;
}

void CollisionHandler::handleWallCollisions(Rectangle* rect, const Map& map, MoveResult& move_result,
                                            double delta_time) {
    Vector2d new_velocity = rect->getVelocity();
    GridEdge grid_edge = GridEdge::NONE;
    Tile collision_tile;
    Vector2d new_position = rect->getPosition();

    if (checkWallCollisions(rect, map, grid_edge, collision_tile)) {
        Vector2d wall_normal = grid_edge.toNormal();
        new_velocity = new_velocity - (new_velocity * wall_normal) * wall_normal * 2;
        double collision_time = computeWallCollisionPosition(rect, collision_tile, grid_edge, delta_time);
        new_position = new_position + new_velocity * delta_time * collision_time;
    }

    move_result.setUpdatedVelocity(new_velocity);
    move_result.setNewPosition(new_position);
}

void CollisionHandler::checkCollisions(Rectangle* rect, const Map& map,
                                       const std::vector<std::unique_ptr<Entity> >& entities,
                                       double delta_time) {
    MoveResult move_result(rect->getVelocity(), rect->getPosition());
    handleWallCollisions(rect, map, move_result, delta_time);

    // Check for entity collision
    /*for (const auto& other_entity: entities) {
        if (other_entity.get() == rect) continue;

        checkEntityCollisions(rect, other_entity.get());
    }*/

    rect->setMoveResult(move_result);
}
