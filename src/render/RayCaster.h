#pragma once

#include "SFML/Graphics.hpp"
#include<iostream>
#include "../GameConstants.h"
#include "../math/GameMath.h"

//rayacster using DDA algorithm
namespace GosRender {

    struct RayResponse {
        float distance;
        float collision_position;
        bool is_edge;
    };

    class Player {
    public:
        explicit Player(sf::Vector2f position) : player_position(position) {}

        Player() = default;

        sf::Vector2f player_position;

        const float fieldOfView = PI / 4.f;

        float playerAngle = 0.f;

        sf::Vector2f player_dir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));

        float distanceToProjectionPlane = (0.5f) / tanf(fieldOfView / 2.f);

        float player_height = screen_height / 2;

        float looking_arroy_center = screen_height / 2;
    };

    class Map {
    public:
        Map(std::string map, int map_width, int map_height) : map(map), map_width(map_width), map_height(map_height) {}

        Map() = default;

        std::string map;

        int map_width;

        int map_height;
    };


    void Render(GosRender::Player &, GosRender::Map &);


}















































