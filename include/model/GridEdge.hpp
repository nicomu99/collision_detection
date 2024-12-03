//
// Created by nico on 12/3/24.
//

#ifndef GRIDEDGE_HPP
#define GRIDEDGE_HPP
#include "Vector2d.hpp"

struct GridEdge {
    enum Value {
        NONE, TOP, BOTTOM, LEFT, RIGHT
    };

    Value value;

    GridEdge(Value v): value(v) { }

    [[nodiscard]] Vector2d toNormal() const {
        switch(value) {
            case TOP:
                return {0, -1};
            case BOTTOM:
                return {0, 1};
            case LEFT:
                return {-1, 0};
            case RIGHT:
                return {1, 0};
            case NONE:
                default:
                    return {0, 0};
        }
    }
};

#endif //GRIDEDGE_HPP
