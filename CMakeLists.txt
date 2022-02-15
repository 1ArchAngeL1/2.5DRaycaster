
cmake_minimum_required(VERSION 3.15)

set(PROJECT_NAME RayCaster)

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 20)

find_package(SFML COMPONENTS system window graphics network audio)

file(
        GLOB_RECURSE SOURCE_FILES
        src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/MazeGeneration.cpp src/MazeGeneration.h src/GameConstants.cpp src/GameConstants.h src/math/GameMath.cpp src/math/GameMath.h)

target_link_libraries(${PROJECT_NAME} sfml-graphics sfml-window sfml-system sfml-network)