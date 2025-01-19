//
// Created by nico on 1/19/25.
//
#include "RectangleCircleCollision.hpp"

#include <algorithm>

#include "Vector2d.hpp"
#include "Rectangle.hpp"
#include "Circle.hpp"


namespace RectangleCircleCollision {
    void handleCollision(const Rectangle* rect, const Circle* circle, MoveResult& move_result,
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
}
