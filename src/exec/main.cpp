#include<iostream>


#include "../render/RayCaster.h"

int main() {
    std::string map;

    map += "11111111111111111111111111111111111111111111111111";
    map += "1................................................1";
    map += "11111111111111111..111111111111111111111111111...1";
    map += "1............................................1...1";
    map += "1............................................1...1";
    map += "1............................................1...1";
    map += "1............................................1...1";
    map += "1............................................1...1";
    map += "1.....1......111111111111....11111....111..111...1";
    map += "1....111.....1..........1....11111....1..........1";
    map += "1...11111....1..........1....11111....1..........1";
    map += "1....111.....1..........1....11111....1..........1";
    map += "1.....1......1..........1....11111....1..........1";
    map += "1............1..........1.............1..........1";
    map += "1............1..........1.............1..........1";
    map += "11111111111111..........111111111111111..........1";
    map += "1............1..........1.............1..........1";
    map += "1............1..........1.............1..........1";
    map += "1.....1......1..........1.............1..........1";
    map += "1....111.....1..........1.............1..........1";
    map += "1.....1......1..........1.............1..........1";
    map += "1............1..........1.............1..........1";
    map += "1............1..........1.............1..........1";
    map += "1............1..........1.............1..........1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1......111111111111111111........................1";
    map += "1......1................1........................1";
    map += "1......1.........................................1";
    map += "1......1.........................................1";
    map += "1......1................1........................1";
    map += "1......111111111111111111........................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "111111111111111111111111...11111111111111111111111";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "1................................................1";
    map += "11111111111111111111111111111111111111111111111111";


    GosRender::Player game_player;
    game_player.player_position = sf::Vector2f(2,1.5f);
    GosRender::Map game_map;

    game_map.map = map;
    game_map.map_height = 50;
    game_map.map_width = 50;

    //render.SetMap(map, 50, 50);

    //render.SetPosition(2, 1.5f);

    //render.SetPosition(46, 11);
    GosRender::Render(game_player,game_map);


    return 0;
}