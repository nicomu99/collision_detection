//
// Created by nico on 11/30/24.
//

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include <vector>
#include "Entity.hpp"
#include "Vector2d.hpp"

class Rectangle : public Entity {
    float width;
    float height;

    double top;
    double bottom;
    double left;
    double right;

    std::vector<Vector2d> corner_points;

public:
    Rectangle();
    Rectangle(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width,
              float rectangle_height, std::string id);

    [[nodiscard]] float getWidth() const;
    [[nodiscard]] float getHeight() const;
    [[nodiscard]] double getTop() const;
    [[nodiscard]] double getBottom() const;
    [[nodiscard]] double getLeft() const;
    [[nodiscard]] double getRight() const;
    [[nodiscard]] const std::vector<Vector2d>& getCornerPoints() const;

    void calculateCornerPoints(std::vector<Vector2d>& points, Vector2d center) const;
    void calculateCornerPointsAndSetBounds(std::vector<Vector2d>& points, Vector2d center);

    void move(Vector2d target) override;
    void update() override;
    void revertMove() override;

    void handleWallCollisions(const Map& map, MoveResult& move_result, double delta_time) override;
    void checkEntityCollisions(Entity* other_entity, MoveResult& move_result) override;

    void handleCollision(Circle* circle, MoveResult& move_result) override;
    void handleCollision(Rectangle* rectangle, MoveResult& move_result) override;

    bool isCollision(const Rectangle* rectangle) override;
    bool isCollision(const Circle* circle) override;
};

#endif //RECTANGLE_HPP
