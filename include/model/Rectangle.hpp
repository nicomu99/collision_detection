//
// Created by nico on 11/30/24.
//

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include "Entity.hpp"

class Rectangle: public Entity {
    float rectangle_width;
    float rectangle_height;

public:
    Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width, float rectangle_height);

    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;

};

#endif //RECTANGLE_HPP
