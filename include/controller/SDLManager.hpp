//
// Created by nico on 11/29/24.
//

#ifndef SDLMANAGER_HPP
#define SDLMANAGER_HPP
#include <SDL3/SDL.h>

class SDLManager {
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
public:
    SDLManager();
    ~SDLManager();

    [[nodiscard]] SDL_Renderer* getRenderer() const;
};

#endif //SDLMANAGER_HPP
