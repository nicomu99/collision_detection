//
// Created by nico on 1/19/25.
//

#ifndef RECTANGLECIRCLECOLLISION_HPP
#define RECTANGLECIRCLECOLLISION_HPP

class MoveResult;
class Circle;
class Rectangle;

namespace RectangleCircleCollision {
    void handleCollision(const Rectangle* rect, const Circle* circle, MoveResult& move_result, bool determine_rectangle_velocity);
}

#endif //RECTANGLECIRCLECOLLISION_HPP
