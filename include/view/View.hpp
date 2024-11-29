//
// Created by nico on 11/29/24.
//

#ifndef VIEW_HPP
#define VIEW_HPP
#include <SDL_render.h>

class SDLManager;
class Model;

class View {
    SDL_Renderer* renderer;

public:
    View(const SDLManager& sdl_manager);

    void render(const Model& model) const;
    void renderCircle() const;
    void renderRectangle() const;
    void renderTriangle() const;
};

#endif //VIEW_HPP
