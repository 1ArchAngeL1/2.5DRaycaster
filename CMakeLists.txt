
cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME RayCaster)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS system window graphics network audio)

file(
        GLOB_RECURSE SOURCE_FILES
        src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/math/GameMath.cpp src/math/GameMath.h map/maze/MazeGeneration.cpp map/maze/MazeGeneration.h src/render/2.5D/2.5DRender.cpp src/render/2.5D/2.5DRender.h src/render/2D/2DRender.cpp src/render/2D/2DRender.h)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-network)