//
// Created by nico on 11/29/24.
//
#include "View.hpp"

#include <cmath>
#include <iostream>

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
    std::vector<Vector2d> corner_points{};
    rect->calculateCornerPoints(corner_points, rect->getInterpolatedPosition(alpha));
    struct Edge {
        double x0, y0, x1, y1;
    };
    std::vector<Edge> edges = {
        {corner_points[0].x, corner_points[0].y, corner_points[1].x, corner_points[1].y},
        {corner_points[1].x, corner_points[1].y, corner_points[2].x, corner_points[2].y},
        {corner_points[2].x, corner_points[2].y, corner_points[3].x, corner_points[3].y},
        {corner_points[3].x, corner_points[3].y, corner_points[0].x, corner_points[0].y},
    };
    for(const Edge& edge: edges) {
        SDL_RenderLine(renderer, edge.x0, edge.y0, edge.x1, edge.y1);
    }
}
