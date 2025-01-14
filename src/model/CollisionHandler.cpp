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

void CollisionHandler::projectRectangleOntoAxis(const Rectangle* rect, const Vector2d& axis, double& min, double& max) {
    // Initialize min and max with the projection of the first vertex
    const std::vector<Vector2d>& rect_corner_points = rect->getCornerPoints();
    double projection = rect_corner_points[0].x * axis.x + rect_corner_points[0].y * axis.y;
    min = max = projection;

    // Loop through the remaining vertices
    for (int i = 1; i < 4; ++i) {
        projection = rect_corner_points[i].x * axis.x + rect_corner_points[i].y * axis.y;
        if (projection < min) min = projection;
        if (projection > max) max = projection;
    }
}

void CollisionHandler::computeAxes(const Rectangle* rect, std::vector<Vector2d>& edges) {
    const std::vector<Vector2d>& vertices = rect->getCornerPoints();
    for (int i = 0; i < 4; i++) {
        Vector2d edge = vertices[i] - vertices[i + 1];
        edge.to_normal();
        edge.normalize();
        edges.push_back(edge);
    }
}

bool CollisionHandler::intervalsOverlap(double min_a, double max_a, double min_b, double max_b) {
    return max_a >= min_b && max_b >= min_a;
}

double CollisionHandler::computeOverlap(double min_a, double max_a, double min_b, double max_b) {
    return std::min(max_a, max_b) - std::max(min_a, min_b);
}

void CollisionHandler::handleCollision(const Rectangle* rect, const Rectangle* other_rect, MoveResult& move_result) {
    // Calculate the direction vector of the edge
    std::vector<Vector2d> edges;
    computeAxes(rect, edges);
    computeAxes(other_rect, edges);

    double minimal_overlap = std::numeric_limits<double>::max();
    Vector2d mtv = edges[0];
    for (int i = 0; i < 4; i++) {
        double min_a, max_a, min_b, max_b;
        projectRectangleOntoAxis(rect, edges[i], min_a, max_a);
        projectRectangleOntoAxis(other_rect, edges[i], min_b, max_b);

        double overlap = computeOverlap(min_a, max_a, min_b, max_b);
        if (overlap < minimal_overlap) {
            mtv = edges[i];
            minimal_overlap = overlap;
        }
    }

    Vector2d rect_pos = rect->getPosition();
    Vector2d other_pos = other_rect->getPosition();
    Vector2d direction = other_pos - rect_pos;
    if (direction.dot(mtv) < 0) {
        mtv = -mtv;
    }

    Vector2d translation = mtv * minimal_overlap / 2.0;
    Vector2d new_pos = rect_pos - translation;
    move_result.setNewPosition(new_pos);

    Vector2d v_rect = rect->getVelocity();
    Vector2d v_normal_rect = mtv * v_rect.dot(mtv);
    Vector2d v_tan_rect = v_rect - v_normal_rect;

    Vector2d v_other = other_rect->getVelocity();
    Vector2d v_normal_other = mtv * v_other.dot(mtv);

    double rect_mass = rect->getMass();
    double other_mass = other_rect->getMass();
    Vector2d v_rect_new = (v_normal_rect * (rect_mass - other_mass) + v_normal_other * other_mass * 2) / (
                              rect_mass + other_mass) + v_tan_rect;
    move_result.setUpdatedVelocity(v_rect_new);
}

void CollisionHandler::handleCollision(const Rectangle* rect, const Circle* circle, MoveResult& move_result,
                                       bool determine_rectangle_velocity) {
    Vector2d v_rect = rect->getVelocity();
    Vector2d v_circle = circle->getVelocity();

    Vector2d pos_circle = circle->getPosition();

    // Determine collision normal
    double p_x = std::clamp(pos_circle.x, rect->getLeft(), rect->getRight());
    double p_y = std::clamp(pos_circle.y, rect->getTop(), rect->getBottom());
    Vector2d collision_point = Vector2d(p_x, p_y);
    Vector2d collision_normal = pos_circle - collision_point;
    collision_normal /= collision_normal.length(); // make it a unit vector

    // Determine new velocity
    double v_rect_normal_mag = v_rect.dot(collision_normal);
    double v_circle_normal_mag = v_circle.dot(collision_normal);

    Vector2d v_rect_normal = collision_normal * v_rect_normal_mag;
    Vector2d v_circle_normal = collision_normal * v_circle_normal_mag;

    Vector2d v_rect_tangential = v_rect - v_rect_normal;
    Vector2d v_circle_tangential = v_circle - v_circle_normal;

    double m_rect = rect->getMass();
    double m_circle = circle->getMass();
    double new_rect_normal_mag = ((m_rect - m_circle) * v_rect_normal_mag + 2.0 * m_circle * v_circle_normal_mag)
                                 / (m_rect + m_circle);
    double new_circle_normal_mag = (2.0 * m_rect * v_rect_normal_mag + (m_circle - m_rect) * v_circle_normal_mag)
                                   / (m_rect + m_circle);

    Vector2d v_rect_n_prime = collision_normal * new_rect_normal_mag;
    Vector2d v_circle_n_prime = collision_normal * new_circle_normal_mag;

    Vector2d v_rect_new = v_rect_tangential + v_rect_n_prime;
    Vector2d v_circle_new = v_circle_tangential + v_circle_n_prime;

    // Determine new position
    double penetration_depth = circle->getRadius() - (pos_circle - collision_point).length();

    double total_mass = m_rect + m_circle;
    double rect_correction_factor = m_circle / total_mass;
    double circle_correction_factor = m_rect / total_mass;

    Vector2d correction = collision_normal * penetration_depth;
    Vector2d rect_correction = correction * rect_correction_factor;
    Vector2d circle_correction = correction * circle_correction_factor;

    Vector2d rect_pos = rect->getPosition() - rect_correction;
    Vector2d circle_pos = circle->getPosition() + circle_correction;

    if (determine_rectangle_velocity) {
        move_result.setUpdatedVelocity(v_rect_new);
        move_result.setNewPosition(rect_pos);
    } else {
        move_result.setUpdatedVelocity(v_circle_new);
        move_result.setNewPosition(circle_pos);
    }
}

void CollisionHandler::handleCollision(const Circle* circle, const Circle* other_circle, Vector2d& velocity) {
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

bool CollisionHandler::isWallCollision(const Rectangle* rect, const Map& map,
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

bool CollisionHandler::isWallCollision(const Circle* circle, const Map& map, Vector2d& wall_normal, Tile& collision_tile) {
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

double CollisionHandler::computeWallCollisionPosition(const Rectangle* rect, const Tile& tile, GridEdge grid_edge,
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
                collision_time = (tile.getBottom() - rect->getTop()) / (delta_time * rect_velocity.y);
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

void CollisionHandler::handleWallCollisions(const Rectangle* rect, const Map& map, MoveResult& move_result,
                                            double delta_time) {
    Vector2d new_velocity = rect->getVelocity();
    GridEdge grid_edge = GridEdge::NONE;
    Vector2d new_position = rect->getPosition();

    if (Tile collision_tile; isWallCollision(rect, map, grid_edge, collision_tile)) {
        Vector2d wall_normal = grid_edge.toNormal();
        new_velocity = new_velocity - (new_velocity * wall_normal) * wall_normal * 2;
        double collision_time = computeWallCollisionPosition(rect, collision_tile, grid_edge, delta_time);
        new_position = new_position - new_velocity * delta_time * collision_time;
    }

    move_result.setUpdatedVelocity(new_velocity);
    move_result.setNewPosition(new_position);
}

double CollisionHandler::computeWallCollisionPosition(const Circle* circle, const Tile& tile, Vector2d wall_normal,
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

void CollisionHandler::handleWallCollisions(const Circle* circle, const Map& map, MoveResult& move_result,
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

void CollisionHandler::checkEntityCollisions(Rectangle* rect, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (rect->isCollision(other_rect)) {
            handleCollision(rect, other_rect, move_result);
        }
    } else if (auto circle = dynamic_cast<Circle*>(other_entity)) {
        if (rect->isCollision(circle)) {
            handleCollision(rect, circle, move_result, true);
        }
    }
}

void CollisionHandler::checkEntityCollisions(Circle* circle, Entity* other_entity, MoveResult& move_result) {
    // First resolve what type of entity it is
    Vector2d velocity = circle->getVelocity();
    if (auto other_rect = dynamic_cast<Rectangle*>(other_entity)) {
        if (circle->isCollision(other_rect)) {
            handleCollision(other_rect, circle, move_result, false);
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
