//
// Created by nico on 11/30/24.
//
#include "Rectangle.hpp"

#include <cmath>
#include <iostream>

#include "SDLManager.hpp"

Rectangle::Rectangle()
    : Entity({0, 0}, 0, {0, 0}, 0), width(0),
      height(0), top(0), bottom(0), left(0), right(0), corner_points(std::vector<Vector2d>()) {
}

Rectangle::Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width,
                     float rectangle_height)
    : Entity(position, rotation, velocity, speed), width(rectangle_width),
      height(rectangle_height), top(0), bottom(0), left(0), right(0), corner_points(std::vector<Vector2d>()) {
    Rectangle::calculateCornerPoints(corner_points, current_position);
}

float Rectangle::getWidth() const {
    return width;
}

float Rectangle::getHeight() const {
    return height;
}

double Rectangle::getTop() const {
    return top;
}

double Rectangle::getBottom() const {
    return bottom;
}

double Rectangle::getLeft() const {
    return left;
}

double Rectangle::getRight() const {
    return right;
}

const std::vector<Vector2d>& Rectangle::getCornerPoints() const {
    return corner_points;
}

void Rectangle::calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) {
    points.clear();

    std::vector<Vector2d> corners = {
        {-width / 2, -height / 2},
        {-width / 2, height / 2},
        {width / 2, height / 2},
        {width / 2, -height / 2}
    };

    const double radians = rotation * M_PI / 180.0f;
    const auto cos_a = cos(radians);
    const auto sin_a = sin(radians);

    bottom = 0;
    top = ScreenConstants::SCREEN_HEIGHT;
    left = ScreenConstants::SCREEN_WIDTH;
    right = 0;
    for (const auto& point: corners) {
        double corner_x = center.x + point.x * cos_a - point.y * sin_a;
        double corner_y = center.y + point.x * sin_a + point.y * cos_a;

        left = std::min(left, corner_x);
        right = std::max(right, corner_x);
        top = std::min(top, corner_y);
        bottom = std::max(bottom, corner_y);

        points.emplace_back(corner_x, corner_y);
    }
}

void Rectangle::move(Vector2d target) {
    previous_position = current_position;
    current_position += target;
    calculateCornerPoints(corner_points, current_position);
}
