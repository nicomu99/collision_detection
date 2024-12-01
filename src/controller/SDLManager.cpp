//
// Created by nico on 11/29/24.
//
#include "SDLManager.hpp"

#include <stdexcept>
#include <SDL3/SDL_hints.h>

SDLManager::SDLManager() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        throw std::runtime_error("SDL could not initialize. Error: " + std::string(SDL_GetError()) + "\n");
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    window = SDL_CreateWindow("Collision Detection",  SCREEN_WIDTH, SCREEN_HEIGHT , NULL);
    if(!window) {
        SDL_Quit();
        throw std::runtime_error("SDL Window could not be initialized. Error: " + std::string(SDL_GetError()) + "\n");
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if(!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("SDL Renderer could not be initialized. Error: " + std::string(SDL_GetError()) + "\n");
    }
}

SDLManager::~SDLManager() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

SDL_Renderer* SDLManager::getRenderer() const {
    return renderer;
}
