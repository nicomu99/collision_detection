//
// Created by nico on 11/29/24.
//

#ifndef SDLMANAGER_HPP
#define SDLMANAGER_HPP
#include <SDL3/SDL.h>

namespace ScreenConstants {
    static constexpr int SCREEN_WIDTH = 2560;
    static constexpr int SCREEN_HEIGHT = 1820;
}

class SDLManager {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
public:
    SDLManager();
    ~SDLManager();

    [[nodiscard]] SDL_Renderer* getRenderer() const;
};

#endif //SDLMANAGER_HPP
