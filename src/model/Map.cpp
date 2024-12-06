//
// Created by nico on 12/2/24.
//
#include "Map.hpp"
#include <cmath>
#include <iostream>

Map::Map() : map(ROWS, std::vector<Tile>(COLS)) {
    constexpr auto size = static_cast<double>(TILE_SIZE);
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            double pos_x = (col * size) + size / 2.0;
            double pos_y = (row * size) + size / 2.0;
            TileType type = (row == 0 || col == 0 || row == ROWS - 1 || col == COLS - 1) ? WALL : GRASS;
            map[row][col] = Tile({pos_x, pos_y}, 0, {0, 0}, 0, TILE_SIZE, TILE_SIZE, type);
        }
    }
}

const std::vector<std::vector<Tile> >& Map::getMap() const {
    return map;
}

const Tile& Map::getTile(int x, int y) const {
    x = x / TILE_SIZE;
    y = y / TILE_SIZE;

    if (x < 0) {
        x = 0;
    } else if (x >= COLS) {
        x = COLS - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= ROWS) {
        y = ROWS - 1;
    }

    return map[y][x];
}

bool Map::isWallAt(int x, int y) const {
    x = x / TILE_SIZE;
    y = y / TILE_SIZE;

    if (x < 0) {
        x = 0;
    } else if (x >= COLS) {
        x = COLS - 1;
    }

    if (y < 0) {
        y = 0;
    } else if (y >= ROWS) {
        y = ROWS - 1;
    }

    return map[y][x].getTileType() == WALL;
}

bool Map::isWallAt(double x, double y) const {
    int x_coordinate = static_cast<int>(std::round(x / TILE_SIZE));
    int y_coordinate = static_cast<int>(std::round(y / TILE_SIZE));

    if (x_coordinate < 0 || x_coordinate >= COLS || y_coordinate < 0 || y_coordinate >= ROWS) {
        return true;
    }

    return map[y_coordinate][x_coordinate].getTileType() == WALL;
}
