//
// Created by nico on 12/2/24.
//
#include "Tile.hpp"

Tile::Tile(): tile_type(GRASS) {
}

Tile::Tile(Vector2d position, int rotation, Vector2d velocity, double speed, float rectangle_width,
           float rectangle_height, TileType tile_type)
    : Rectangle(position, rotation, velocity, speed, rectangle_width, rectangle_height, "Tile"), tile_type(tile_type) {
}

TileType Tile::getTileType() const {
    return tile_type;
}
