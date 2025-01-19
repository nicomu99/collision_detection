//
// Created by nico on 1/19/25.
//

#ifndef CIRCLECIRCLECOLLISION_HPP
#define CIRCLECIRCLECOLLISION_HPP

class Circle;
class MoveResult;

namespace CircleCircleCollision {
    void handleCollision(const Circle* circle, const Circle* other_circle, MoveResult& move_result);
}

#endif //CIRCLECIRCLECOLLISION_HPP
