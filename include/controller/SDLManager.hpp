//
// Created by nico on 11/29/24.
//

#ifndef SDLMANAGER_HPP
#define SDLMANAGER_HPP
#include <SDL2/SDL.h>

class SDLManager {
    static constexpr int SCREEN_WIDTH = 1280;
    static constexpr int SCREEN_HEIGHT = 960;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
public:
    SDLManager();
    ~SDLManager();

    [[nodiscard]] SDL_Renderer* getRenderer() const;
};

#endif //SDLMANAGER_HPP
