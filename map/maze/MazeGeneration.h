//
// Created by Leo Gogichaishvili on 16.02.22.
//

#ifndef RAYCASTER_MAZEGENERATION_H
#define RAYCASTER_MAZEGENERATION_H

#include <string>

namespace GosRender {

    struct MazeBlock {
        MazeBlock * right = NULL;
        MazeBlock * left = NULL;
        MazeBlock * up = NULL;
        MazeBlock * down = NULL;
        bool visited = false;
    };

    std::string MazeToString(MazeBlock * maze,int map_width,int map_height);

    std::string GenerateMaze(const int & map_width, const int & map_height);

}





#endif //RAYCASTER_MAZEGENERATION_H
