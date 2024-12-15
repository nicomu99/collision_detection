//
// Created by nico on 11/30/24.
//
#include "Rectangle.hpp"

#include <cmath>
#include <iostream>
#include <utility>

#include "Constants.hpp"

Rectangle::Rectangle()
    : Entity({0, 0}, 0, {0, 0}, 0, "no_id"), width(0),
      height(0), top(0), bottom(0), left(0), right(0), corner_points(std::vector<Vector2d>()) {
}

Rectangle::Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width,
                     float rectangle_height, std::string id)
    : Entity(position, rotation, velocity, speed, std::move(id)), width(rectangle_width),
      height(rectangle_height), top(0), bottom(0), left(0), right(0), corner_points(std::vector<Vector2d>()) {
    calculateCornerPointsAndSetBounds(corner_points, current_position);
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

void Rectangle::calculateCornerPointsAndSetBounds(std::vector<Vector2d>& points, Vector2d center) {
    points.clear();
    calculateCornerPoints(points, center);

    bottom = 0;
    top = Constants::SCREEN_HEIGHT;
    left = Constants::SCREEN_WIDTH;
    right = 0;

    for(const auto& point: points) {
        left = std::min(left, point.x);
        right = std::max(right, point.x);
        top = std::min(top, point.y);
        bottom = std::max(bottom, point.y);
    }
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


    for (const auto& point: corners) {
        double corner_x = center.x + point.x * cos_a - point.y * sin_a;
        double corner_y = center.y + point.x * sin_a + point.y * cos_a;

        points.emplace_back(corner_x, corner_y);
    }
}

void Rectangle::move(Vector2d target) {
    previous_position = current_position;
    current_position += target;
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}

void Rectangle::revertMove() {
    current_position = previous_position;
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}

void Rectangle::update() {
    this->velocity = move_result.getUpdatedVelocity();
    this->current_position = move_result.getNewPosition();
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}
