//
// Created by nico on 11/29/24.
//
#include "View.hpp"

#include "Circle.hpp"
#include "Model.hpp"
#include "Rectangle.hpp"
#include "SDLManager.hpp"

View::View(const SDLManager& sdl_manager): renderer(sdl_manager.getRenderer()) {
}

void View::render(const Model& model, double alpha) const {
    SDL_RenderClear(renderer);

    renderMap(model.getMap());
    for (const auto& entity: model.getEntities()) {
        if (auto rect = dynamic_cast<Rectangle*>(entity.get())) {
            renderRectangle(rect, alpha);
        } else if (auto circle = dynamic_cast<Circle*>(entity.get())) {
            renderCircle(circle, alpha);
        }
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderPresent(renderer);
}

void View::renderMap(const Map& map) const {
    auto tile_map = map.getMap();
    for (const auto& row: tile_map) {
        for (const auto& col: row) {
            if (col.getTileType() == WALL) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            }
            SDL_FRect render_rect(static_cast<float>(col.getLeft()), static_cast<float>(col.getTop()), col.getWidth(),
                                  col.getHeight());
            SDL_RenderFillRect(renderer, &render_rect);
        }
    }
}

void View::renderRectangle(const Rectangle* rect, double alpha) const {
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
    for (const Edge& edge: edges) {
        SDL_RenderLine(renderer, static_cast<float>(edge.x0), static_cast<float>(edge.y0), static_cast<float>(edge.x1),
                       static_cast<float>(edge.y1));
    }
}

void View::renderCircle(const Circle* circle, double alpha) const {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    double radius = circle->getRadius();
    Vector2d center = circle->getInterpolatedPosition(alpha);
    auto center_x = static_cast<float>(center.x);
    auto center_y = static_cast<float>(center.y);

    float x = 0.0f;
    auto y = static_cast<float>(radius);
    auto decision = static_cast<float>(1.0f - radius);
    while(x <= y) {
        SDL_RenderPoint(renderer, center_x + x, center_y + y);
        SDL_RenderPoint(renderer, center_x - x, center_y + y);
        SDL_RenderPoint(renderer, center_x + x, center_y - y);
        SDL_RenderPoint(renderer, center_x - x, center_y - y);
        SDL_RenderPoint(renderer, center_x + y, center_y + x);
        SDL_RenderPoint(renderer, center_x + y, center_y - x);
        SDL_RenderPoint(renderer, center_x - y, center_y + x);
        SDL_RenderPoint(renderer, center_x - y, center_y - x);

        x += 1;
        if (decision <= 0) {
            decision += 2 * x + 1;
        } else {
            y -= 1;
            decision += 2 * (x - y) + 1;
        }
    }
}
