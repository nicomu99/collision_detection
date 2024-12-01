cmake_minimum_required(VERSION 3.29)
project(collision_detection)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL3 REQUIRED sdl3)

include_directories(${PROJECT_SOURCE_DIR}/include/controller
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
        src/model/PhysicsEngine.cpp)

target_link_libraries(collision_detection ${SDL3_LIBRARIES})