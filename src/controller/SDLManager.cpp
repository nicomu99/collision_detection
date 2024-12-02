//
// Created by nico on 11/29/24.
//
#include "SDLManager.hpp"

#include <iostream>
#include <stdexcept>
#include <SDL3/SDL_hints.h>

SDLManager::SDLManager() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        throw std::runtime_error("SDL could not initialize. Error: " + std::string(SDL_GetError()) + "\n");
    }

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");

    SDL_CreateWindowAndRenderer("Collision Detection", ScreenConstants::SCREEN_WIDTH, ScreenConstants::SCREEN_HEIGHT, 0, &window, &renderer);
    if(!window) {
        SDL_Quit();
        throw std::runtime_error("SDL Window could not be initialized. Error: " + std::string(SDL_GetError()) + "\n");
    }

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
