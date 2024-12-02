#include <chrono>
#include <iostream>
#include <thread>

#include "Controller.hpp"
#include "SDLManager.hpp"

int main() {
    try {
        Controller controller;
        bool running = true;

        using clock = std::chrono::high_resolution_clock;
        const std::chrono::duration<double> target_frame_duration(1.0 / 60.0);
        double accumulator = 0.0;
        auto last_time = clock::now(); // Initial time point

        while (running) {
            // Time calculation
            auto current_time = clock::now();
            std::chrono::duration<double> delta_time = current_time - last_time;
            last_time = current_time;

            accumulator += delta_time.count();

            controller.handleInput(running);
            // controller.updateModel(delta_time.count());

            while (accumulator >= target_frame_duration.count()) {
                controller.updateModel(target_frame_duration.count());
                accumulator -= target_frame_duration.count();
            }

            double alpha = accumulator / target_frame_duration.count();
            controller.renderView(alpha);

            // FPS calculation (optional)
            double fps = 1.0 / delta_time.count();
            // std::cout << "Delta time: " << delta_time << " seconds | FPS: " << fps << std::endl;
        }
    } catch (std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}
