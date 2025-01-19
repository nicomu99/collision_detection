//
// Created by nico on 1/19/25.
//
#include "RectangleRectangleCollision.hpp"

#include <limits>

#include "Vector2d.hpp"
#include "Rectangle.hpp"

namespace RectangleRectangleCollision {
    void projectRectangleOntoAxis(const Rectangle* rect, const Vector2d& axis, double& min,
                                  double& max) {
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

    void computeAxes(const Rectangle* rect, std::vector<Vector2d>& edges) {
        const std::vector<Vector2d>& vertices = rect->getCornerPoints();
        for (int i = 0; i < 4; i++) {
            Vector2d edge = vertices[i] - vertices[i + 1];
            edge.to_normal();
            edge.normalize();
            edges.push_back(edge);
        }
    }

    bool intervalsOverlap(double min_a, double max_a, double min_b, double max_b) {
        return max_a >= min_b && max_b >= min_a;
    }

    double computeOverlap(double min_a, double max_a, double min_b, double max_b) {
        return std::min(max_a, max_b) - std::max(min_a, min_b);
    }

    void handleCollision(const Rectangle* rect, const Rectangle* other_rect,
                         MoveResult& move_result) {
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
}
