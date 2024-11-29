#include <iostream>

#include "Controller.hpp"
#include "SDLManager.hpp"

int main() {
    try {
        Controller controller;
        bool running = true;
        Uint64 current_tick = SDL_GetPerformanceCounter();
        Uint64 last_tick = 0;
        double delta_time = 0;
        Uint64 frequency = SDL_GetPerformanceFrequency();

        while(running) {
            last_tick = current_tick;
            current_tick = SDL_GetPerformanceCounter();

            delta_time = static_cast<double>(current_tick - last_tick) / static_cast<double>(frequency);

            controller.handleInput(running);
            controller.updateModel(delta_time);
            controller.renderView();
        }

    } catch(std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
