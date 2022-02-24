#include<iostream>


#include "../render/RayCaster.h"
#include "../../map/maze/MazeGeneration.h"
#include "../render/2D/2DRender.h"
#include "../render/2.5D/2.5DRender.h"


int main() {


    std::string map;
    //map = GosRender::GenerateMaze(10, 10);

    GosRender::TopViewPlayer game_player(sf::Vector2f(1.5f, 1.5f), 0);
    //game_player.SetPosition(sf::Vector2f(1.5f, 1.5f));
    GosRender::Map game_map;
    map = "11111111111111111111";
    map.append("1.1..1......1...1..1");
    map.append("1...........1......1");
    map.append("1..1...1..1........1");
    map.append("1.1...1........1...1");
    map.append("1...1..1....1......1");
    map.append("1.1............1...1");
    map.append("1.....11...1.......1");
    map.append("1...1..............1");
    map.append("1..11...1......1...1");
    map.append("1.1..1...1..1..1...1");
    map.append("1.1..1.............1");
    map.append("1.......1...1.1.1..1");
    map.append("1..1...1...........1");
    map.append("1.1...1.....1......1");
    map.append("1...1..1.......1...1");
    map.append("1.1............1...1");
    map.append("1.....11.1..1......1");
    map.append("1...1.......1..1...1");
    map.append("11111111111111111111");


    game_map.map = map;
    game_map.map_height = 20;
    game_map.map_width = 20;

//    GosRender::Map mapi;
//    mapi.map = map;
//    mapi.map_width = 30;
//    mapi.map_height = 30;

    GosRender::RenderTopView(game_player, game_map);


    return 0;
}
