//
// Created by nico on 11/30/24.
//
#include "Rectangle.hpp"

Rectangle::Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, int rectangle_width,
                     int rectangle_height)
    : Entity(position, rotation, velocity, speed), rectangle_width(rectangle_width),
      rectangle_height(rectangle_height) {
}

int Rectangle::getWidth() const {
    return rectangle_width;
}

int Rectangle::getHeight() const {
    return rectangle_height;
}
