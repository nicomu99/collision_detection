//
// Created by nico on 11/30/24.
//
#include "Rectangle.hpp"

#include <cmath>
#include <iostream>
#include <utility>

#include "Circle.hpp"
#include "CollisionHandler.hpp"
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

    for (const auto& point: points) {
        left = std::min(left, point.x);
        right = std::max(right, point.x);
        top = std::min(top, point.y);
        bottom = std::max(bottom, point.y);
    }
}

void Rectangle::calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) const {
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
    Entity::move(target);
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}

void Rectangle::update() {
    Entity::update();
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}

void Rectangle::revertMove() {
    Entity::revertMove();
    calculateCornerPointsAndSetBounds(corner_points, current_position);
}

void Rectangle::handleWallCollisions(const Map& map, MoveResult& move_result, double delta_time) {
    CollisionHandler::handleWallCollisions(this, map, move_result, delta_time);
}

void Rectangle::checkEntityCollisions(Entity* other_entity, MoveResult& move_result) {
    CollisionHandler::checkEntityCollisions(this, other_entity, move_result);
}

bool Rectangle::isCollision(const Rectangle* rectangle) {
    return this->getRight() > rectangle->getLeft() && this->getLeft() <= rectangle->getRight() &&
       this->getBottom() > rectangle->getTop() && this->getTop() <= rectangle->getBottom();
}

bool Rectangle::isCollision(const Circle* circle) {
    Vector2d c = circle->getPosition();
    double radius = circle->getRadius();

    double closest_x = std::max(this->getLeft(), std::min(c.x, this->getRight()));
    double closest_y = std::max(this->getTop(), std::min(c.y, this->getBottom()));

    return c.euclidean(Vector2d(closest_x, closest_y)) < radius;
}
