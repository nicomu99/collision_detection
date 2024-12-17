//
// Created by nico on 12/8/24.
//
#include <CollisionHandler.hpp>
#include <iostream>
#include <limits>
#include <algorithm>

#include "Entity.hpp"
#include "GridEdge.hpp"
#include "Map.hpp"
#include "MoveResult.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"
#include "Tile.hpp"

void CollisionHandler::handleCollision(Rectangle* rect, Rectangle* other_rect, Vector2d& velocity) {
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

    double normal_component = rect->getVelocity().dot(collision_normal);
    velocity = velocity - collision_normal * normal_component * 2;
}

void CollisionHandler::handleCollision(Rectangle* rect, Circle* circle, Vector2d& velocity) {
    Vector2d circle_position = circle->getPosition();

    double p_x = std::clamp(circle_position.x, rect->getLeft(), rect->getRight());
    double p_y = std::clamp(circle_position.y, rect->getTop(), rect->getBottom());

    Vector2d collision_point = {p_x, p_y};
    Vector2d collision_normal = (collision_point - circle_position);
    collision_normal /= collision_normal.length();

    double normal_component = velocity.dot(collision_normal);
    velocity = velocity - collision_normal * normal_component * 2;
}

void CollisionHandler::handleCollision(Circle* circle, Rectangle* rect, Vector2d& velocity) {
    Vector2d circle_position = circle->getPosition();

    double p_x = std::clamp(circle_position.x, rect->getLeft(), rect->getRight());
    double p_y = std::clamp(circle_position.y, rect->getTop(), rect->getBottom());

    Vector2d collision_point = {p_x, p_y};
    Vector2d collision_normal = (collision_point - circle_position);
    collision_normal /= collision_normal.length();

    velocity = rect->getVelocity() * collision_normal * collision_normal;
}

bool CollisionHandler::isEntityCollision(const Rectangle* rect, const Rectangle* other_rect) {
    return rect->getRight() > other_rect->getLeft() && rect->getLeft() <= other_rect->getRight() &&
           rect->getBottom() > other_rect->getTop() && rect->getTop() <= other_rect->getBottom();
}

bool CollisionHandler::isEntityCollision(const Circle* circle, const Rectangle* rect) {
    Vector2d c = circle->getPosition();
    double radius = circle->getRadius();

    double closest_x = std::max(rect->getLeft(), std::min(c.x, rect->getRight()));
    double closest_y = std::max(rect->getTop(), std::min(c.y, rect->getBottom()));

    return c.euclidean(Vector2d(closest_x, closest_y)) < radius;
}

void CollisionHandler::checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    Vector2d velocity = rect->getVelocity();
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (isEntityCollision(rect, other_rect)) {
            handleCollision(rect, other_rect, velocity);
            move_result.setUpdatedVelocity(velocity);
        }
    } else if (auto circle = dynamic_cast<Circle*>(other_entity)) {
        if (isEntityCollision(circle, rect)) {
            handleCollision(rect, circle, velocity);
            move_result.setUpdatedVelocity(velocity);
        }
    }
}

void CollisionHandler::checkEntityCollisions(Circle* circle, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    Vector2d velocity = circle->getVelocity();
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (isEntityCollision(circle, other_rect)) {
            handleCollision(circle, other_rect, velocity);
            move_result.setUpdatedVelocity(velocity);
        }
    }
}

bool CollisionHandler::isWallCollision(Rectangle* rect, const Map& map,
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

bool CollisionHandler::isWallCollision(Circle* circle, const Map& map, GridEdge& hit_edge, Tile& collision_tile) {
    double radius = circle->getRadius();
    Vector2d position = circle->getPosition();
    auto min_x = static_cast<int>(position.x - radius);
    auto max_x = static_cast<int>(position.x + radius);
    auto min_y = static_cast<int>(position.y - radius);
    auto max_y = static_cast<int>(position.y + radius);

    bool collision_detected = false;
    for (int x = min_x; x <= max_x; x++) {
        for (int y = min_y; y <= max_y; y++) {
            if (map.isWallAt(x, y) && position.euclidean(Vector2d(x, y)) < radius) {
                const Tile& tile = map.getTile(x, y);
                collision_detected = true;

                if (position.x - radius <= tile.getRight() && circle->getVelocity().x < 0) {
                    hit_edge = GridEdge::RIGHT;
                } else if (position.x + radius >= tile.getLeft() && circle->getVelocity().x > 0) {
                    hit_edge = GridEdge::LEFT;
                } else if (position.y - radius <= tile.getBottom() && circle->getVelocity().y < 0) {
                    hit_edge = GridEdge::BOTTOM;
                } else if (position.y + radius >= tile.getTop() && circle->getVelocity().y > 0) {
                    hit_edge = GridEdge::TOP;
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
    Vector2d new_position = rect->getPosition();

    if (Tile collision_tile; isWallCollision(rect, map, grid_edge, collision_tile)) {
        Vector2d wall_normal = grid_edge.toNormal();
        new_velocity = new_velocity - (new_velocity * wall_normal) * wall_normal * 2;
        double collision_time = computeWallCollisionPosition(rect, collision_tile, grid_edge, delta_time);
        new_position = new_position + new_velocity * delta_time * collision_time;
    }

    move_result.setUpdatedVelocity(new_velocity);
    move_result.setNewPosition(new_position);
}

void CollisionHandler::handleWallCollisions(Circle* circle, const Map& map, MoveResult& move_result,
                                            double delta_time) {
    Vector2d new_velocity = circle->getVelocity();
    GridEdge grid_edge = GridEdge::NONE;
    Vector2d new_position = circle->getPosition();

    if (Tile collision_tile; isWallCollision(circle, map, grid_edge, collision_tile)) {
        Vector2d wall_normal = grid_edge.toNormal();
        new_velocity = new_velocity - (new_velocity * wall_normal) * wall_normal * 2;
        // double collision_time = computeWallCollisionPosition(circle, collision_tile, grid_edge, delta_time);
        double collision_time = 1;
        new_position = new_position + new_velocity * delta_time * collision_time;
    }

    move_result.setUpdatedVelocity(new_velocity);
    move_result.setNewPosition(new_position);
}

void CollisionHandler::checkCollisions(Entity* entity, const Map& map,
                                       const std::vector<std::unique_ptr<Entity> >& entities,
                                       double delta_time) {
    MoveResult move_result(entity->getVelocity(), entity->getPosition());

    if (auto rect = dynamic_cast<Rectangle*>(entity)) {
        handleWallCollisions(rect, map, move_result, delta_time);

        // Check for entity collision
        for (const auto& other_entity: entities) {
            if (other_entity.get() == rect) continue;

            checkEntityCollisions(rect, other_entity.get(), move_result);
        }
    } else if (auto circle = dynamic_cast<Circle*>(entity)) {
        handleWallCollisions(circle, map, move_result, delta_time);

        for (const auto& other_entity: entities) {
            if (other_entity.get() == circle) continue;

            checkEntityCollisions(circle, other_entity.get(), move_result);
        }
    }

    entity->setMoveResult(move_result);
}
