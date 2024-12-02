//
// Created by nico on 12/2/24.
//

#ifndef MAP_HPP
#define MAP_HPP
#include <vector>

#include "Tile.hpp"
#include "SDLManager.hpp"

class Map {
    static constexpr int TILE_SIZE = 20;
    static constexpr int ROWS = ScreenConstants::SCREEN_HEIGHT / TILE_SIZE;
    static constexpr int COLS = ScreenConstants::SCREEN_WIDTH / TILE_SIZE;
    std::vector<std::vector<Tile>> map;

public:
    Map();

    [[nodiscard]] const std::vector<std::vector<Tile>>& getMap() const;
    bool isWallAt(int x, int y) const;
};

#endif //MAP_HPP
