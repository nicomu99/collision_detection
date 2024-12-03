//
// Created by nico on 11/29/24.
//

#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "Model.hpp"
#include "SDLManager.hpp"
#include "View.hpp"

class Controller {
    SDLManager sdl_manager;
    Model model;
    View view;

public:
    Controller();

    [[nodiscard]] const SDLManager& getSDLManager() const;

    static void handleInput(bool& running);
    void renderView(double alpha) const;
    void updateModel(double delta_time);
};

#endif //CONTROLLER_HPP
