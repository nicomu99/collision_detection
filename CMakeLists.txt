cmake_minimum_required(VERSION 3.29)
project(collision_detection)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/controller
        ${PROJECT_SOURCE_DIR}/include/view
        ${PROJECT_SOURCE_DIR}/include/model)

set(CMAKE_CXX_STANDARD 20)

add_executable(collision_detection main.cpp
        include/controller/SDLManager.hpp
        src/controller/SDLManager.cpp
        include/controller/Controller.hpp
        src/controller/Controller.cpp
        include/model/Model.hpp
        include/view/View.hpp
        src/model/Model.cpp
        src/view/View.cpp
        include/model/Entity.hpp
        src/model/Entity.cpp
        include/model/Vector2d.hpp
        include/model/Rectangle.hpp
        src/model/Rectangle.cpp
        include/model/PhysicsEngine.hpp
        src/model/PhysicsEngine.cpp
        include/model/Map.hpp
        src/model/Map.cpp
        include/model/Tile.hpp
        src/model/Tile.cpp
        include/Constants.hpp
        include/model/CollisionHandler.hpp
        src/model/CollisionHandler.cpp
        include/model/MoveResult.hpp
        src/model/MoveResult.cpp
        include/model/Circle.hpp
        src/model/Circle.cpp
        include/model/CircleWallCollision.hpp
        include/model/CircleWallCollision.hpp
        src/model/CircleWallCollision.cpp
        include/model/RectangleWallCollision.hpp
        src/model/RectangleWallCollision.cpp
        include/model/RectangleRectangleCollision.hpp
        src/model/RectangleRectangleCollision.cpp
        include/model/RectangleCircleCollision.hpp
        src/model/RectangleCircleCollision.cpp
        include/model/CircleCircleCollision.hpp
        src/model/CircleCircleCollision.cpp)

target_link_libraries(collision_detection ${SDL3_LIBRARIES})