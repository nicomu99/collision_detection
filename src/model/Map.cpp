//
// Created by nico on 12/2/24.
//
#include "Map.hpp"
#include <cmath>
#include <iostream>

Map::Map() : map(ROWS, std::vector<Tile>(COLS)) {
    constexpr auto size = static_cast<double>(TILE_SIZE);
    const auto top_tile = Tile({static_cast<double>(Constants::SCREEN_WIDTH) / 2, static_cast<double>(TILE_SIZE) / 2},
                               0, {0, 0}, 0, Constants::SCREEN_WIDTH, TILE_SIZE, WALL);
    const auto bottom_tile = Tile({
                                      static_cast<double>(Constants::SCREEN_WIDTH) / 2,
                                      Constants::SCREEN_HEIGHT - static_cast<double>(TILE_SIZE) / 2
                                  },
                                  0, {0, 0}, 0, Constants::SCREEN_WIDTH, TILE_SIZE, WALL);
    const auto left_tile = Tile({
                                    static_cast<double>(TILE_SIZE) / 2,
                                    static_cast<double>(Constants::SCREEN_HEIGHT) / 2
                                },
                                0, {0, 0}, 0, TILE_SIZE, Constants::SCREEN_HEIGHT, WALL);
    const auto right_tile = Tile({
                                     static_cast<double>(Constants::SCREEN_WIDTH) - static_cast<double>(TILE_SIZE) / 2,
                                     static_cast<double>(Constants::SCREEN_HEIGHT) / 2
                                 },
                                 0, {0, 0}, 0, TILE_SIZE, Constants::SCREEN_HEIGHT, WALL);
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            double pos_x = (col * size) + size / 2.0;
            double pos_y = (row * size) + size / 2.0;
            if (row == 0) {
                map[row][col] = top_tile;
            } else if (row == ROWS - 1) {
                map[row][col] = bottom_tile;
            } else if (col == 0) {
                map[row][col] = left_tile;
            } else if (col == COLS - 1) {
                map[row][col] = right_tile;
            } else {
                map[row][col] = Tile({pos_x, pos_y}, 0, {0, 0}, 0, TILE_SIZE, TILE_SIZE, GRASS);
            }
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

const Tile& Map::getTile(double x, double y) const {
    int cast_x = (int) x;
    int cast_y = (int) y;

    return getTile(cast_x, cast_y);
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
