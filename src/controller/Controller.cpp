//
// Created by nico on 11/29/24.
//

#include "Controller.hpp"

#include "Model.hpp"
#include "View.hpp"

Controller::Controller(): sdl_manager(SDLManager()), model(Model()), view(View(sdl_manager)) { }

const SDLManager& Controller::getSDLManager() const {
    return sdl_manager;
}

void Controller::handleInput(bool& running) {
    SDL_Event event;

    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
}

void Controller::renderView(double alpha) const {
    view.render(model, alpha);
}

void Controller::updateModel(double delta_time) {
    model.update(delta_time);
}
