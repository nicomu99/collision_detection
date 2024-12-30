//
// Created by nico on 12/8/24.
//
#include <CollisionHandler.hpp>
#include <iostream>
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

void CollisionHandler::handleCollision(Rectangle* rect, Circle* circle, Vector2d& velocity,
                                       bool determine_rectangle_velocity) {
    // ------------------------------------------------------------------------
    // 1. Get current velocities and positions
    // ------------------------------------------------------------------------
    Vector2d vRect = rect->getVelocity(); // velocity of rectangle
    Vector2d vCircle = circle->getVelocity(); // velocity of circle

    Vector2d posRect = rect->getPosition(); // position of rectangle (e.g., center)
    Vector2d posCircle = circle->getPosition(); // position of circle (center)

    // ------------------------------------------------------------------------
    // 2. Compute the collision normal
    //    For a more realistic approach, find the actual contact normal.
    //    Here we do a simple center-to-center vector from rectangle -> circle.
    // ------------------------------------------------------------------------
    Vector2d collisionNormal = posCircle - posRect;
    collisionNormal /= collisionNormal.length(); // make it a unit vector

    // ------------------------------------------------------------------------
    // 3. Decompose velocities into normal & tangential components
    // ------------------------------------------------------------------------
    // Dot products with the normal
    double vRect_n = vRect.dot(collisionNormal);
    double vCircle_n = vCircle.dot(collisionNormal);

    // Tangential components (subtract normal component from total velocity)
    // vt = v - (v.n)n
    Vector2d vRect_t = vRect - collisionNormal * vRect_n;
    Vector2d vCircle_t = vCircle - collisionNormal * vCircle_n;

    // ------------------------------------------------------------------------
    // 4. Resolve collision (perfectly elastic, equal masses)
    //    => The normal components just swap
    // ------------------------------------------------------------------------
    double vRect_n_prime = vCircle_n; // rectangle’s new normal velocity
    double vCircle_n_prime = vRect_n; // circle’s new normal velocity

    // ------------------------------------------------------------------------
    // 5. Reconstruct the final velocities
    // ------------------------------------------------------------------------
    Vector2d vRectFinal = vRect_t + collisionNormal * vRect_n_prime;
    Vector2d vCircleFinal = vCircle_t + collisionNormal * vCircle_n_prime;

    // ------------------------------------------------------------------------
    // 7. Update the 'velocity' output parameter as requested
    // ------------------------------------------------------------------------
    if (determine_rectangle_velocity) {
        velocity = vRectFinal;
    } else {
        velocity = vCircleFinal;
    }
}

void CollisionHandler::handleCollision(Circle* circle, Circle* other_circle, Vector2d& velocity) {
    Vector2d c1 = circle->getPosition();
    Vector2d c2 = other_circle->getPosition();

    // Compute collision normal and normalize it
    Vector2d collision_normal = (c1 - c2);
    collision_normal /= collision_normal.length();

    // Compute normal components (scalar values)
    double normal_component = circle->getVelocity().dot(collision_normal);
    double normal_component_other = other_circle->getVelocity().dot(collision_normal);

    // Compute tangential velocity
    Vector2d tangential_velocity = circle->getVelocity() - collision_normal * normal_component;

    // Compute resulting velocity after collision
    velocity = collision_normal * normal_component_other + tangential_velocity;
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
    Vector2d previous_position = circle->getPreviousPosition();
    Vector2d current_position = circle->getPosition();

    double old_left = previous_position.x - radius;
    double old_right = previous_position.x + radius;
    double old_top = previous_position.y - radius;
    double old_bottom = previous_position.y + radius;

    double new_left = current_position.x - radius;
    double new_right = current_position.x + radius;
    double new_top = current_position.y - radius;
    double new_bottom = current_position.y + radius;

    bool collision_detected = false;
    for (int x = static_cast<int>(new_left); x <= new_right; x += Map::TILE_SIZE) {
        for (int y = static_cast<int>(new_top); y <= new_bottom; y += Map::TILE_SIZE) {
            if (map.isWallAt(x, y)) {
                collision_detected = true;
                std::cout << "Old top: " << old_top << " New top: " << new_top << std::endl;

                const Tile& tile = map.getTile(x, y);
                if (old_right < tile.getLeft() && new_right >= tile.getLeft()) {
                    hit_edge = GridEdge::LEFT;
                } else if (old_left > tile.getRight() && new_left <= tile.getRight()) {
                    hit_edge = GridEdge::RIGHT;
                } else if (old_bottom < tile.getTop() && new_bottom >= tile.getTop()) {
                    hit_edge = GridEdge::BOTTOM;
                } else if (old_top > tile.getBottom() && new_top <= tile.getBottom()) {
                    hit_edge = GridEdge::TOP;
                }
            }
        }
    }

    std::cout << "Collision detected: " << collision_detected << std::endl;
    std::cout << hit_edge.value << std::endl;

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

void CollisionHandler::checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    Vector2d velocity = rect->getVelocity();
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (rect->isCollision(other_rect)) {
            handleCollision(rect, other_rect, velocity);
            move_result.setUpdatedVelocity(velocity);
        }
    } else if (auto circle = dynamic_cast<Circle*>(other_entity)) {
        if (rect->isCollision(circle)) {
            handleCollision(rect, circle, velocity, true);
            move_result.setUpdatedVelocity(velocity);
        }
    }
}

void CollisionHandler::checkEntityCollisions(Circle* circle, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    Vector2d velocity = circle->getVelocity();
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (circle->isCollision(other_rect)) {
            handleCollision(other_rect, circle, velocity, false);
            move_result.setUpdatedVelocity(velocity);
        }
    } else if (auto other_circle = dynamic_cast<Circle*>(other_entity)) {
        if (circle->isCollision(other_circle)) {
            handleCollision(circle, other_circle, velocity);
            move_result.setUpdatedVelocity(velocity);
        }
    }
}

void CollisionHandler::checkCollisions(Entity* entity, const Map& map,
                                       const std::vector<std::unique_ptr<Entity> >& entities,
                                       double delta_time) {
    MoveResult move_result(entity->getVelocity(), entity->getPosition());

    entity->handleWallCollisions(map, move_result, delta_time);
    for (const auto& other_entity: entities) {
        if (other_entity.get() == entity) continue;

        entity->checkEntityCollisions(other_entity.get(), move_result);
    }

    entity->setMoveResult(move_result);
}
