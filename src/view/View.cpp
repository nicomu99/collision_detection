//
// Created by nico on 11/29/24.
//
#include "View.hpp"

#include "Model.hpp"
#include "SDLManager.hpp"

View::View(const SDLManager& sdl_manager): renderer(sdl_manager.getRenderer()) { }

void View::render(const Model& model) const {
    SDL_RenderClear(renderer);


    SDL_RenderPresent(renderer);
}
