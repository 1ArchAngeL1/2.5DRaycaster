//
// Created by Leo Gogichaishvili on 22.02.22.
//

#include "2DRender.h"


static void DrawTiles(sf::RenderWindow &window, sf::RectangleShape *tiles, int size) {
    for (int tile_index = 0; tile_index < size; tile_index++)window.draw(tiles[tile_index]);
}

static void InitTiles(sf::RectangleShape *tiles, GosRender::Map &map, sf::Vector2f start_pos) {
    for (int i = 0; i < map.map_height; i++) {
        for (int j = 0; j < map.map_width; j++) {
            int index = i * map.map_width + j;
            tiles[index].setPosition(
                    start_pos + sf::Vector2f(j * tiles[index].getSize().x, i * tiles[index].getSize().y));
            map.map[index] == '1' ? tiles[index].setFillColor(sf::Color::Yellow) : tiles[index].setFillColor(
                    sf::Color::Black);
        }
    }
}

static void InitialiseRays(std::vector<sf::RectangleShape> &rays, int num_rays) {
    for (int i = 0; i < num_rays; i++) {
        rays.push_back(sf::RectangleShape());
    }
}


static void CastTopViewRays(GosRender::TopViewPlayer &player, GosRender::Map &map, std::vector<sf::RectangleShape> &rays) {
    float angle_from = player.GetAngle() - player.GetFieldOfView() * 0.5f;
    float angle_to = player.GetAngle() + player.GetFieldOfView() * 0.5f;
    int counter = 0;
    for (float gradus = angle_from; gradus < angle_to; gradus += 0.001f) {
        GosRender::RayResponse response = CastRay(&player, gradus, map);
        float distance =
                response.distance * (static_cast<float>(GosRender::map_width) / static_cast<float>(map.map_width));
        if (distance >= GosRender::map_width * 0.4f)distance = GosRender::map_width * 0.4f;
        rays[counter].setSize(sf::Vector2f(1, distance));
        rays[counter].setPosition(player.GetPositionOnRender(map));
        rays[counter].setFillColor(sf::Color::White);
        rays[counter].setRotation(-GosRender::RadianToGradus(gradus) + 270);
        ++counter;
    }
}


static bool CheckForCollision(GosRender::TopViewPlayer &player, GosRender::Map &map, sf::Vector2f movement) {
    for (int angle = 0.f; angle < 360.f; angle += 15.f) {
        sf::Vector2f test_position =
                player.GetPosition() +
                (sf::Vector2f(cosf(angle), sinf(angle)) *
                 (player.PLAYER_RADIUS / ((float) GosRender::map_width / (float) map.map_width)) +
                 movement);
        if (map.map[static_cast<int>(test_position.y) * map.map_width + static_cast<int>(test_position.x)] == '1')
            return true;
    }
    return false;
}


void GosRender::RenderTopView(GosRender::TopViewPlayer &player, GosRender::Map &map) {

    float scale_x = GosRender::map_width / map.map_width;
    float scale_y = GosRender::map_height / map.map_height;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;
    sf::RenderWindow window(sf::VideoMode(GosRender::screen_width, GosRender::screen_height), "2D TopView",
                            sf::Style::Default, settings);

    sf::RectangleShape map_wallpaper(sf::Vector2f(GosRender::map_width, GosRender::map_height));
    map_wallpaper.setPosition(GosRender::map_position);
    map_wallpaper.setPosition(map_position);
    map_wallpaper.setFillColor(sf::Color::Black);


    sf::RectangleShape *map_tiles = new sf::RectangleShape[map.map_width * map.map_height];

    for (int tile_index = 0; tile_index < map.map_width * map.map_height; tile_index++) {
        map_tiles[tile_index].setSize(sf::Vector2f(scale_x, scale_y));
    }
    InitTiles(map_tiles, map, GosRender::map_position);

    std::vector<sf::RectangleShape> rays;

    InitialiseRays(rays, 1800);

    sf::Clock clock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        float time_passed = clock.restart().asSeconds();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            if (!CheckForCollision(player, map, sf::Vector2f(1, 0) * 2.f * time_passed)) {
                player.Move(sf::Vector2f(1, 0), 2.f, time_passed, map);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            if (!CheckForCollision(player, map, sf::Vector2f(-1, 0) * 2.f * time_passed)) {
                player.Move(sf::Vector2f(-1, 0), 2.f, time_passed, map);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            if (!CheckForCollision(player, map, sf::Vector2f(0, -1) * 2.f * time_passed)) {
                player.Move(sf::Vector2f(0, -1), 2.f, time_passed, map);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            if (!CheckForCollision(player, map, sf::Vector2f(0, 1) * 2.f * time_passed)) {
                player.Move(sf::Vector2f(0, 1), 2.f, time_passed, map);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.ChangleAnge(4.5f * time_passed);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.ChangleAnge(-4.5f * time_passed);
        }

        CastTopViewRays(player, map, rays);

        window.clear();
        window.draw(map_wallpaper);
        DrawTiles(window, map_tiles, map.map_width * map.map_height);
        for (int i = 0; i < rays.size(); i++)window.draw(rays[i]);
        player.DrawPlayer(window, map);
        window.display();

    }
    delete[] map_tiles;
}
