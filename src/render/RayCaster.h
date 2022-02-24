#pragma once

#include "SFML/Graphics.hpp"
#include<iostream>
#include "../math/GameMath.h"
#include <array>

//rayacster using DDA algorithm
namespace GosRender {

    extern const int screen_width;

    extern const int screen_height;

    extern const int map_width;

    extern const int map_height;

    extern const sf::Vector2f map_position;

    struct RayResponse {
        float distance;
        float collision_position;
        bool is_edge;
    };

    struct Map {
    public:
        Map(std::string map, int map_width, int map_height) : map(map), map_width(map_width), map_height(map_height) {}

        Map() = default;

        std::string map;

        int map_width;

        int map_height;
    };

    class Player {
    protected:
        sf::Vector2f position;
        sf::Vector2f player_dir;
        float field_of_view = PI / 4.f;
        float angle;
    public:
        void Move(sf::Vector2f direction, float speed, float delta_time);

        Player(sf::Vector2f position, float angle) : position(position), angle(angle) {
            player_dir = sf::Vector2f(cosf(angle), sinf(angle));
        }

        void SetPosition(const sf::Vector2f &);

        void ChangleAnge(const float &dx);

        sf::Vector2f GetDirection() const;

        sf::Vector2f GetPosition() const;

        float GetFieldOfView() const;

        float GetAngle() const;
    };

    //
    //

    class FirstPersonPlayer : public Player {
    public:
        explicit FirstPersonPlayer(sf::Vector2f position, float angle) : Player(position, angle) {
            distanceToProjectionPlane = 0.5f / tanf(field_of_view * 0.5f);
            player_height = screen_height * 0.5f;
            projection_center = screen_height * 0.5f;
        }

        FirstPersonPlayer() = default;

        float distanceToProjectionPlane;

        float player_height;

        float projection_center;
    };

    //
    //

    class TopViewPlayer : public Player {
    public:

        explicit TopViewPlayer(sf::Vector2f position, float angle) : Player(position, angle) {
            this->player_shape.setRadius(PLAYER_RADIUS);
            this->player_shape.setFillColor(sf::Color::Red);
            field_of_view = PI / 2.f;
        }

        void DrawPlayer(sf::RenderWindow &window, const GosRender::Map &map);

        void SetPosition(const sf::Vector2f &, const Map &);

        sf::Vector2f GetPositionOnRender(const GosRender::Map &);

        void Move(sf::Vector2f, float, float, const GosRender::Map &);

        static constexpr float PLAYER_RADIUS = 10.f;

    private:

        const sf::Vector2f center_offset = sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS);

        void UpdatePlayerPosition(const GosRender::Map &);

        sf::CircleShape player_shape;

    };


    RayResponse CastRay(GosRender::Player *player, float ray_angle, GosRender::Map map);

}















































