//
// Created by nico on 12/15/24.
//
#include "Circle.hpp"

Circle::Circle()
    : Entity({0, 0}, 0, {0, 0}, 0, "no_id"), radius(0) {
}

Circle::Circle(Vector2d position, int rotation, Vector2d velocity, double speed, double radius, std::string id)
    : Entity(position, rotation, velocity, speed, std::move(id)), radius(radius) {
}

double Circle::getRadius() const {
    return radius;
}
