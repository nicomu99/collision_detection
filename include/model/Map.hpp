//
// Created by nico on 12/2/24.
//

#ifndef MAP_HPP
#define MAP_HPP
#include <vector>

#include "Constants.hpp"
#include "Tile.hpp"

class Map {
    std::vector<std::vector<Tile>> map;

public:
    static constexpr int TILE_SIZE = 20;
    static constexpr int ROWS = Constants::SCREEN_HEIGHT / TILE_SIZE;
    static constexpr int COLS = Constants::SCREEN_WIDTH / TILE_SIZE;

    Map();

    [[nodiscard]] const std::vector<std::vector<Tile>>& getMap() const;
    [[nodiscard]] const Tile& getTile(int x, int y) const;
    [[nodiscard]] const Tile& getTile(double x, double y) const;

    [[nodiscard]] bool isWallAt(int x, int y) const;
    [[nodiscard]] bool isWallAt(double x, double y) const;
};

#endif //MAP_HPP
