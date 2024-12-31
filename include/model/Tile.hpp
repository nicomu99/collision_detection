//
// Created by nico on 12/2/24.
//

#ifndef TILE_HPP
#define TILE_HPP
#include "Rectangle.hpp"

enum TileType {
    WALL, GRASS
};

class Tile : public Rectangle {
    TileType tile_type;

public:
    Tile();
    Tile(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width,
         float rectangle_height, TileType tile_type);

    [[nodiscard]] TileType getTileType() const;
    [[nodiscard]] bool isWall() const;
};

#endif //TILE_HPP
