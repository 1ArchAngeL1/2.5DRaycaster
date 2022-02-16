//
// Created by Leo Gogichaishvili on 16.02.22.
//

#include "MazeGeneration.h"
#include <vector>
#include <iostream>

static constexpr std::pair<int, int> moves[]{{1,  0},
                                             {-1, 0},
                                             {0,  1},
                                             {0,  -1}};


std::string GosRender::MazeToString(MazeBlock *maze, int map_width, int map_height) {
    std::string maze_str = "";
    maze_str += '\n';
    for (int i = 0; i < map_height; i++) {
        for (int j = 0; j < map_width; j++) {
            maze_str += '1';
            if (maze[i * map_width + j].up != NULL) {
                maze_str += '0';
            } else {
                maze_str += '1';
            }
            maze_str += '1';
        }
        maze_str += "\n";
        for (int j = 0; j < map_width; j++) {
            if (maze[i * map_width + j].left != NULL) {
                maze_str += '0';
            } else {
                maze_str += '1';
            }
            maze_str += '0';
            if (maze[i * map_width + j].right != NULL) {
                maze_str += '0';
            } else {
                maze_str += '1';
            }
        }
        maze_str += "\n";
        for (int j = 0; j < map_width; j++) {
            maze_str += '1';
            if (maze[i * map_width + j].down != NULL) {
                maze_str += '0';
            } else {
                maze_str += '1';
            }
            maze_str += '1';
        }
        maze_str += "\n";
    }
    return maze_str;
}

static std::vector<int> RandomShuffle() {
    srand(time(NULL));
    std::vector<int> indexes{0, 1, 2, 3};
    for (int i = 0; i < 4; i++) {
        int index = rand() % 4;
        std::swap(indexes[i], indexes[index]);
    }
    return indexes;
}

bool CheckValid(int curr_x, int curr_y, int map_width, int map_height) {
    return (curr_x >= 0 && curr_x < map_width && curr_y >= 0 &&
            curr_y < map_height && curr_y * map_width + curr_x < map_width * map_height);
}

GosRender::MazeBlock *Generate(GosRender::MazeBlock *maze, int map_width, int map_height, int curr_x, int curr_y) {
    //if (maze[curr_y * map_width + curr_x].visited)return NULL;
    std::vector<int> indexes = RandomShuffle();
    GosRender::MazeBlock *current = &maze[curr_y * map_width + curr_x];
    current->visited = true;
    for (int i = 0; i < 4; i++) {
        std::pair<int, int> move = moves[indexes[i]];
        if (!CheckValid(curr_x + move.first, curr_y + move.second, map_width, map_height))continue;
        GosRender::MazeBlock *next = &maze[((curr_y + move.second) * map_width) + curr_x + move.first];
        if (!next->visited) {
            if (move.first == 1) {
                current->right = next;
                next->left = current;
            } else if (move.first == -1) {
                current->left = next;
                next->right = current;
            } else if (move.second == 1) {
                current->down = next;
                next->up = current;
            } else if (move.second == -1) {
                current->up = next;
                next->down = current;
            }
            Generate(maze, map_width, map_height, curr_x + move.first, curr_y + move.second);
            next->visited = true;
        }
    }
    return current;

}


std::string GosRender::GenerateMaze(const int &map_width, const int &map_height) {
    MazeBlock *maze = new MazeBlock[map_width * map_height];
    Generate(maze, map_width, map_height, 0, 0);
    std::cout << "aee" << std::endl;
    return MazeToString(maze, map_width, map_height);
}