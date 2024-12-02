//
// Created by nico on 11/30/24.
//

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include <vector>

#include "Entity.hpp"

class Rectangle: public Entity {
    float width;
    float height;

    double top;
    double bottom;
    double left;
    double right;

    std::vector<Vector2d> corner_points;

public:
    Rectangle();
    Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width, float rectangle_height);

    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;
    [[nodiscard]] double getTop() const;
    [[nodiscard]] double getBottom() const;
    [[nodiscard]] double getLeft() const;
    [[nodiscard]] double getRight() const;
    [[nodiscard]] const std::vector<Vector2d>& getCornerPoints() const;

    void calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) override;
    void calculateCornerPointsAndSetBounds(std::vector<Vector2d>& points, Vector2d center);
    void move(Vector2d target) override;
};

#endif //RECTANGLE_HPP
