#include<iostream>


#include "../render/RayCaster.h"
#include "../maze/MazeGeneration.h"

int main() {


    std::string map;

    map = GosRender::GenerateMaze(5, 5);

    GosRender::Player game_player;
    game_player.player_position = sf::Vector2f(1.5f, 1.5f);
    GosRender::Map game_map;

    game_map.map = map;
    game_map.map_height = 15;
    game_map.map_width = 15;

    //render.SetMap(map, 50, 50);

    //render.SetPosition(2, 1.5f);

    //render.SetPosition(46, 11);
    GosRender::Render(game_player, game_map);


    return 0;
}
