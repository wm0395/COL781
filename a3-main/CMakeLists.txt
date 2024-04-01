cmake_minimum_required(VERSION 3.0)

project(a3)
set(CMAKE_CXX_STANDARD 11)

find_package(glm REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL_image REQUIRED)

add_library(a3 src/Scene.hpp src/Scene.cpp src/Shape.cpp src/Camera.cpp src/Lighting.cpp deps/src/gl.c)
target_include_directories(a3 PUBLIC deps/include)
target_link_libraries(a3 glm::glm SDL2::SDL2 SDL2_image)

add_executable(example1.1 examples/example1.1.cpp)
target_link_libraries(example1.1 a3)

add_executable(example1.2 examples/example1.2.cpp)
target_link_libraries(example1.2 a3)

add_executable(example1.3 examples/example1.3.cpp)
target_link_libraries(example1.3 a3)

add_executable(example1.4 examples/example1.4.cpp)
target_link_libraries(example1.4 a3)

add_executable(example2.1 examples/example2.1.cpp)
target_link_libraries(example2.1 a3)