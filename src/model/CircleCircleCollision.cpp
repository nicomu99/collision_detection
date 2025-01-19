//
// Created by nico on 1/19/25.
//
#include "CircleCircleCollision.hpp"

#include "Vector2d.hpp"
#include "Circle.hpp"

namespace CircleCircleCollision {
    void handleCollision(const Circle* circle, const Circle* other_circle, MoveResult& move_result) {
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
        Vector2d new_velocity = collision_normal * normal_component_other + tangential_velocity;
        move_result.setUpdatedVelocity(new_velocity);
    }
}
