//
// Created by nico on 11/30/24.
//

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "Entity.hpp"

class Rectangle: public Entity {
    int rectangle_width;
    int rectangle_height;

public:
    Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, int rectangle_width, int rectangle_height);

    [[nodiscard]] int getWidth() const;
    [[nodiscard]] int getHeight() const;

};

#endif //RECTANGLE_HPP
