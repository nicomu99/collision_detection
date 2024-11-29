//
// Created by nico on 11/29/24.
//
#include "SDLManager.hpp"

#include <stdexcept>

SDLManager::SDLManager() {
    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize. Error: " + std::string(SDL_GetError()) + "\n");
    }

    window = SDL_CreateWindow("Collision Detection", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              SCREEN_WIDTH, SCREEN_HEIGHT , SDL_WINDOW_SHOWN);
    if(!window) {
        SDL_Quit();
        throw std::runtime_error("SDL Window could not be initialized. Error: " + std::string(SDL_GetError()) + "\n");
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
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
