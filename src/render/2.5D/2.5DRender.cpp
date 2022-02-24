//
// Created by Leo Gogichaishvili on 22.02.22.
//

#include "2.5DRender.h"





static std::vector<sf::Sprite> textured_walls;

static std::vector<sf::RectangleShape> walls;

static std::vector<sf::RectangleShape> floor_rects;

static std::vector<sf::RectangleShape> ceilling_rects;

static sf::Texture wall_texture;

static sf::Texture floor_texture;

static sf::Texture ceilling_texture;


static void InitFloor(std::vector<sf::RectangleShape> &floor_rects, int clr) {
    for (int i = GosRender::screen_height * 0.5f; i < (int) GosRender::screen_height; i += 3) {
        int color = clr * ((i - (GosRender::screen_height * 0.5f)) / (GosRender::screen_height * 0.5f));
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(GosRender::screen_width, 3));
        shape.setPosition(sf::Vector2f(0, i));
        shape.setFillColor(sf::Color(color, color, color));
        floor_rects.push_back(shape);
    }
}

static void InitCeilling(std::vector<sf::RectangleShape> &ceilling_rects, int clr) {
    for (int i = 0; i < GosRender::screen_height * 0.5f; i += 3) {
        int color = clr - clr * (i / (GosRender::screen_height * 0.5f));
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(GosRender::screen_width, 3));
        shape.setPosition(sf::Vector2f(0, i));
        shape.setFillColor(sf::Color(color, color, color));
        ceilling_rects.push_back(shape);
    }
}

static void MakeDarkSky(std::vector<sf::CircleShape> &ceiling_rects) {
    for (int i = 0; i < GosRender::screen_width; i += 5) {
        sf::CircleShape star(1);
        int starY = rand() % (GosRender::screen_height / 2);
        star.setFillColor(sf::Color::White);
        star.setPosition(i, starY);
        ceiling_rects.push_back(star);
    }
}

void RenderFloor(sf::Texture &floor_texture, sf::Image &target_image, GosRender::FirstPersonPlayer &player) {
    auto floor_image = floor_texture.copyToImage();
    for (int i = player.projection_center; i < GosRender::screen_height; i++) {

        float straight_distance_to_point = player.distanceToProjectionPlane *
                                           (player.player_height / (i - player.projection_center));

        sf::Vector2f left_vector(cosf(player.GetAngle() - (player.GetFieldOfView() * 0.5f)),
                                 sinf(player.GetAngle() - (player.GetFieldOfView() * 0.5f)));

        sf::Vector2f right_vector(cosf(player.GetAngle() + (player.GetFieldOfView() * 0.5f)),
                                  sinf(player.GetAngle() + (player.GetFieldOfView() * 0.5f)));

        float step_x = straight_distance_to_point * (right_vector.x - left_vector.x) / GosRender::screen_width;

        float step_y = straight_distance_to_point * (right_vector.y - left_vector.y) / GosRender::screen_width;

        float floor_x = player.GetPosition().x + straight_distance_to_point * left_vector.x;

        float floor_y = player.GetPosition().y - straight_distance_to_point * left_vector.y;

        for (int j = 0; j < GosRender::screen_width; j++) {
            int cell_x = static_cast<int>(floor_x);
            int cell_y = static_cast<int>(floor_y);

            unsigned int pix_x = (static_cast<int>(floor_image.getSize().x * abs(floor_x - cell_x)) %
                                  (floor_image.getSize().x - 1));
            unsigned int pix_y = (static_cast<int>(floor_image.getSize().y * abs(floor_y - cell_y)) %
                                  (floor_image.getSize().y - 1));

            floor_x += step_x;
            floor_y -= step_y;

            target_image.setPixel(j, i, floor_image.getPixel(pix_x, pix_y));
        }
    }
}

void RenderCeillling(sf::Texture &floor_texture, sf::Image &target_image, GosRender::FirstPersonPlayer &player) {
    auto ceilling_image = floor_texture.copyToImage();
    for (int i = 0; i < player.projection_center; i++) {

        float straight_distance_to_point = player.distanceToProjectionPlane *
                                           (player.player_height / (player.projection_center - i));

        sf::Vector2f left_vector(cosf(player.GetAngle() - (player.GetFieldOfView() * 0.5f)),
                                 sinf(player.GetAngle() - (player.GetFieldOfView() * 0.5f)));

        sf::Vector2f right_vector(cosf(player.GetAngle() + (player.GetFieldOfView() * 0.5f)),
                                  sinf(player.GetAngle() + (player.GetFieldOfView() * 0.5f)));

        float step_x = straight_distance_to_point * (right_vector.x - left_vector.x) / GosRender::screen_width;

        float step_y = straight_distance_to_point * (right_vector.y - left_vector.y) / GosRender::screen_width;

        float floor_x = player.GetPosition().x + straight_distance_to_point * left_vector.x;

        float floor_y = player.GetPosition().y - straight_distance_to_point * left_vector.y;

        for (int j = 0; j < GosRender::screen_width; j++) {
            int cell_x = (int) floor_x;
            int cell_y = (int) floor_y;

            unsigned int pix_x = (static_cast<int>(ceilling_image.getSize().x * abs(floor_x - cell_x)) %
                                  (ceilling_image.getSize().x - 1));
            unsigned int pix_y = (static_cast<int>(ceilling_image.getSize().y * abs(floor_y - cell_y)) %
                                  (ceilling_image.getSize().y - 1));

            floor_x += step_x;
            floor_y -= step_y;

            target_image.setPixel(j, i, ceilling_image.getPixel(pix_x, pix_y));
        }
    }
}


static void LoadTextures() {
    wall_texture.loadFromFile("../resources/wall_texture.jpeg");

    floor_texture.loadFromFile("../resources/wall_texture.jpeg");

    ceilling_texture.loadFromFile("../resources/wall_texture.jpeg");
}

void GosRender::RenderFirstPerson(GosRender::FirstPersonPlayer &player, GosRender::Map &map) {
    sf::Image image;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(GosRender::screen_width, GosRender::screen_height), "2.5D",
                            sf::Style::Default,
                            settings);
    window.setMouseCursorVisible(false);

    window.setFramerateLimit(90);

    LoadTextures();

    float max_distance = sqrt(map.map_height * map.map_height + map.map_width * map.map_width);
    sf::Clock clock;
    float delta_time;
    InitCeilling(ceilling_rects, sf::Color::White.r);
    InitFloor(floor_rects, sf::Color::White.r);
    const sf::Vector2f screen_center = sf::Vector2f((float) screen_width * 0.5f, (float) screen_height * 0.5f);


    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        textured_walls.clear();
        walls.clear();
        delta_time = clock.restart().asSeconds();

//        sf::Vector2f mouse_delta = static_cast<sf::Vector2f>(sf::Mouse::getPosition(window)) - screen_center;
//        sf::Mouse::setPosition((sf::Vector2i)screen_center, window);




        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            player.ChangleAnge(3 * delta_time);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.ChangleAnge(-3 * delta_time);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            float value = player.projection_center - 250.f * delta_time;
            if (value > 0) player.projection_center = value;

        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            float value = player.projection_center + 250.f * delta_time;
            if (value < screen_height) player.projection_center = value;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            float testCollisionX =
                    player.GetPosition().x + static_cast<float>(cos(player.GetAngle()) * 4.5f * delta_time);

            float testCollisionY =
                    player.GetPosition().y - static_cast<float>(sin(player.GetAngle()) * 4.5f * delta_time);

            if (map.map[(int) testCollisionY * map.map_width + (int) testCollisionX] != '1') {
                player.Move(sf::Vector2f(cosf(player.GetAngle()), -sinf(player.GetAngle())), 4.5f, delta_time);
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.Move(sf::Vector2f(-cosf(player.GetAngle()), sinf(player.GetAngle())), 4.5f, delta_time);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player.player_height += 100 * delta_time;
        }

        for (int ray_column = 0; ray_column < GosRender::screen_width; ray_column++) {
            float ray_angle = (player.GetAngle() - (player.GetFieldOfView() * 0.5f) +
                               ((float) ray_column / (float) GosRender::screen_width) * player.GetFieldOfView());

            float dir_x = cosf(ray_angle);
            float dir_y = sinf(ray_angle);

            GosRender::RayResponse respo = CastRay(&player, ray_angle, map);
            float distance_to_wall = respo.distance;
            bool is_edge = respo.is_edge;
            float pos = respo.collision_position;

            float fishEyeAngleFix = CosineOfAngleBetweenVectors(player.GetDirection(),
                                                                sf::Vector2f(dir_x, dir_y));
            distance_to_wall *= fishEyeAngleFix;

            float wall_height = ((player.distanceToProjectionPlane) / distance_to_wall) * GosRender::screen_height;

            float ceilling = (player.projection_center) - (wall_height * 0.5f);

            float floor = GosRender::screen_height - ceilling;

//            sf::RectangleShape wall;
//            wall.setSize(sf::Vector2f(1, wall_height));
//            wall.setPosition(sf::Vector2f(ray_column, ceilling));
//            if (is_edge) {
//                wall.setFillColor(sf::Color::Black);
//            } else {
//                int color = 255 - (distance_to_wall / (float) map.map_width) * 255;
//                wall.setFillColor(sf::Color(color, color, color));
//                sf::Color(255 - (distance_to_wall / (float) map.map_width) * 255,
//                                            255 - (distance_to_wall / (float) map.map_width) * 255,
//                                            255 - (distance_to_wall / (float) map.map_width) * 255);
//            }
            //walls.push_back(wall);

            sf::Sprite wallSprite;
            wallSprite.setTexture(wall_texture);
            wallSprite.setTextureRect(sf::IntRect(pos * wall_texture.getSize().x, 0, 1, wall_texture.getSize().y));
            wallSprite.setScale(sf::Vector2f(1, (wall_height) / wall_texture.getSize().y));
            wallSprite.setPosition(sf::Vector2f(ray_column, ceilling));
            int color = 255 - sqrt((distance_to_wall / max_distance)) * 255;
            wallSprite.setColor(sf::Color(color, color, color));
            textured_walls.push_back(wallSprite);
        }
        image.create(screen_width, screen_height);
        RenderFloor(floor_texture, image, player);
        RenderCeillling(floor_texture, image, player);
        sf::Texture floorBack;
        floorBack.loadFromImage(image);
        sf::Sprite floorSprite;
        floorSprite.setTexture(floorBack);
        floorSprite.setPosition(sf::Vector2f(0, 0));
        window.clear();
        window.draw(floorSprite);
        //for (int i = 0; i < textured_ceilling.size(); i++)window.draw(textured_ceilling[i]);
        // for (int i = 0; i < floor_rects.size(); i++)window.draw(floor_rects[i]);
        for (int i = 0; i < textured_walls.size(); i++)window.draw(textured_walls[i]);
        window.display();
    }
}