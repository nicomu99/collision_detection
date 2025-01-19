//
// Created by nico on 12/15/24.
//
#include "Circle.hpp"

#include "CircleWallCollision.hpp"
#include "CollisionHandler.hpp"
#include "Rectangle.hpp"

Circle::Circle()
    : Entity({0, 0}, 0, {0, 0}, 0, "no_id", 0), radius(0) {
}

Circle::Circle(Vector2d position, int rotation, Vector2d velocity, double speed, double radius, std::string id)
    : Entity(position, rotation, velocity, speed, std::move(id), M_PI * radius * radius), radius(radius) {
}

double Circle::getRadius() const {
    return radius;
}

void Circle::handleWallCollisions(const Map& map, MoveResult& move_result, double delta_time) {
    CircleWallCollision::handleWallCollisions(this, map, move_result, delta_time);
}

void Circle::checkEntityCollisions(Entity* other_entity, MoveResult& move_result) {
    CollisionHandler::checkEntityCollisions(this, other_entity, move_result);
}

bool Circle::isCollision(const Rectangle* rectangle) {
    Vector2d c = this->getPosition();
    double radius = this->getRadius();

    double closest_x = std::max(rectangle->getLeft(), std::min(c.x, rectangle->getRight()));
    double closest_y = std::max(rectangle->getTop(), std::min(c.y, rectangle->getBottom()));

    return c.euclidean(Vector2d(closest_x, closest_y)) < radius;
}

bool Circle::isCollision(const Circle* circle) {
    Vector2d c1 = this->getPosition();
    Vector2d c2 = circle->getPosition();

    // Compute the Euclidean distance between the centers
    double distance = c1.euclidean(c2);

    // Check if the distance is less than the sum of the radii
    return distance < (this->radius + circle->getRadius());
}
