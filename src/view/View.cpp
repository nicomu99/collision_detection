//
// Created by nico on 11/29/24.
//
#include "View.hpp"

#include <cmath>

#include "Model.hpp"
#include "Rectangle.hpp"
#include "SDLManager.hpp"

View::View(const SDLManager& sdl_manager): renderer(sdl_manager.getRenderer()) { }

void View::render(const Model& model, double alpha) const {
    SDL_RenderClear(renderer);

    for(const auto& entity: model.getEntities()) {
        if(auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            renderRectangle(rect, alpha);
        }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderPresent(renderer);
}

void View::renderRectangle(Rectangle* rect, double alpha) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    Vector2d center = rect->getInterpolatedPosition(alpha);
    float width = rect->getWidth();
    float height = rect->getHeight();
    SDL_FRect render_rect = {static_cast<float>(std::round(center.x)), static_cast<float>(std::round(center.y)), width, height};

    SDL_RenderRect(renderer, &render_rect);
}
