//
// Created by nico on 11/29/24.
//
#include "View.hpp"

#include "Model.hpp"
#include "Rectangle.hpp"
#include "SDLManager.hpp"

View::View(const SDLManager& sdl_manager): renderer(sdl_manager.getRenderer()) { }

void View::render(const Model& model) const {
    SDL_RenderClear(renderer);

    for(const auto& entity: model.getEntities()) {
        if(auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            renderRectangle(rect);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void View::renderRectangle(Rectangle* rect) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Vector2d center = rect->getPosition();
    int width = rect->getWidth();
    int height = rect->getHeight();
    SDL_Rect render_rect = {static_cast<int>(center.x), static_cast<int>(center.y), width, height};

    SDL_RenderDrawRect(renderer, &render_rect);
}
