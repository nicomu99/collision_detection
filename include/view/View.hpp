//
// Created by nico on 11/29/24.
//

#ifndef VIEW_HPP
#define VIEW_HPP
#include <SDL3/SDL_render.h>

class Rectangle;
class Circle;
class Map;
class SDLManager;
class Model;

class View {
    SDL_Renderer* renderer;

public:
    explicit View(const SDLManager& sdl_manager);

    void render(const Model& model, double alpha) const;
    void renderMap(const Map& map) const;

    void renderRectangle(const Rectangle* rect, double alpha) const;
    void renderCircle(const Circle* circle, double alpha) const;
    void renderTriangle() const;
};

#endif //VIEW_HPP
