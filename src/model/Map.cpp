//
// Created by nico on 12/2/24.
//
#include "Map.hpp"

Map::Map() : map(ROWS, std::vector<Tile>(COLS)) {
    auto size = static_cast<double>(TILE_SIZE);
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            double pos_x = (col * size) + size / 2.0;
            double pos_y = (row * size) + size / 2.0;
            TileType type = (row == 0 || col == 0 || row == ROWS - 1 || col == COLS - 1) ? WALL : GRASS;
            map[row][col] = Tile({pos_x, pos_y}, 0, {0, 0}, 0, TILE_SIZE, TILE_SIZE, type);
        }
    }
}

const std::vector<std::vector<Tile>>& Map::getMap() const {
    return map;
}

bool Map::isWallAt(int x, int y) const {
    x = x / TILE_SIZE;
    y = y / TILE_SIZE;

    if(x < 0 || y < 0 || x >= COLS || y >= ROWS) {
        return WALL;
    }

    return map[y][x].getTileType() == WALL;
}

