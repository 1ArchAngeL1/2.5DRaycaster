#pragma once

#include "SFML/Graphics.hpp"
#include<iostream>
#include "math.h"
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
        //2d vector that describes direction of the player
        Player () {
            distanceToProjectionPlane = (0.5f) / tanf(fieldOfView / 2.f);
            player_dir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));
        }

        sf::Vector2f player_dir;

        sf::Vector2f player_position;

        float distanceToProjectionPlane;

        const float fieldOfView = PI / 4.f;

        float playerAngle = 0.f;
    };

    class Map {
    public:
        std::string map;

        int map_width;

        int map_height;
    };


    void Render(GosRender::Player&, GosRender::Map&);



}















































