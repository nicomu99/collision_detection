//
// Created by nico on 1/19/25.
//

#ifndef RECTANGLERECTANGLECOLLISION_HPP
#define RECTANGLERECTANGLECOLLISION_HPP
#include <vector>

class MoveResult;
struct Vector2d;
class Rectangle;

namespace RectangleRectangleCollision {
    void projectRectangleOntoAxis(const Rectangle* rect, const Vector2d& axis, double& min, double& max);
    void computeAxes(const Rectangle* rect, std::vector<Vector2d>& edges);
    bool intervalsOverlap(double min_a, double max_a, double min_b, double max_b);
    double computeOverlap(double min_a, double max_a, double min_b, double max_b);

    void handleCollision(const Rectangle* rect, const Rectangle* rectangle, MoveResult& move_result);

}

#endif //RECTANGLERECTANGLECOLLISION_HPP
