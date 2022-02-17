#include "RayCaster.h"


static std::vector<sf::Sprite> textured_walls;

static std::vector<sf::RectangleShape> walls;

static std::vector<sf::RectangleShape> floor_rects;

static std::vector<sf::RectangleShape> ceilling_rects;

static sf::Texture wall_texture;

static sf::Texture floor_texture;

static sf::Texture ceilling_texture;


static void InitFloor(std::vector<sf::RectangleShape> &floor_rects, int clr) {
    for (int i = GosRender::screen_height / 2; i < (int) GosRender::screen_height; i += 3) {
        int color = clr * ((i - (GosRender::screen_height / 2.f)) / (GosRender::screen_height / 2.f));
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(GosRender::screen_width, 3));
        shape.setPosition(sf::Vector2f(0, i));
        shape.setFillColor(sf::Color(color, color, color));
        floor_rects.push_back(shape);
    }
}

static void InitCeilling(std::vector<sf::RectangleShape> &ceilling_rects, int clr) {
    for (int i = 0; i < GosRender::screen_height / 2; i += 3) {
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


void RenderFloor(sf::Texture &floor_texture, sf::Image &target_image, GosRender::Player &player) {
    auto floor_image = floor_texture.copyToImage();
    for (int i = player.projection_center; i < GosRender::screen_height; i++) {

        float straight_distance_to_point = player.distanceToProjectionPlane *
                                           (player.player_height / (i - player.projection_center));

        sf::Vector2f left_vector(cosf(player.player_angle - (player.fieldOfView / 2.f)),
                                 sinf(player.player_angle - (player.fieldOfView / 2.f)));

        sf::Vector2f right_vector(cosf(player.player_angle + (player.fieldOfView / 2.f)),
                                  sinf(player.player_angle + (player.fieldOfView / 2.f)));

        float step_x = straight_distance_to_point * (right_vector.x - left_vector.x) / GosRender::screen_width;

        float step_y = straight_distance_to_point * (right_vector.y - left_vector.y) / GosRender::screen_width;

        float floor_x = player.player_position.x + straight_distance_to_point * left_vector.x;

        float floor_y = player.player_position.y - straight_distance_to_point * left_vector.y;

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

void RenderCeillling(sf::Texture &floor_texture, sf::Image &target_image, GosRender::Player &player) {
    auto ceilling_image = floor_texture.copyToImage();
    for (int i = 0; i < player.projection_center; i++) {

        float straight_distance_to_point = player.distanceToProjectionPlane *
                                           (player.player_height / (player.projection_center - i));

        sf::Vector2f left_vector(cosf(player.player_angle - (player.fieldOfView / 2.f)),
                                 sinf(player.player_angle - (player.fieldOfView / 2.f)));

        sf::Vector2f right_vector(cosf(player.player_angle + (player.fieldOfView / 2.f)),
                                  sinf(player.player_angle + (player.fieldOfView / 2.f)));

        float step_x = straight_distance_to_point * (right_vector.x - left_vector.x) / GosRender::screen_width;

        float step_y = straight_distance_to_point * (right_vector.y - left_vector.y) / GosRender::screen_width;

        float floor_x = player.player_position.x + straight_distance_to_point * left_vector.x;

        float floor_y = player.player_position.y - straight_distance_to_point * left_vector.y;

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

static bool IsEdge(GosRender::Player &player, int box_x, int box_y, sf::Vector2f dir) {
    float angle_diff = player.fieldOfView / static_cast<float>(GosRender::screen_width);
    std::vector<std::pair<float, float >> edges;
    for (int tx = 0; tx < 2; tx++) {
        for (int ty = 0; ty < 2; ty++) {
            int edgeX = box_x + tx;
            int edgeY = box_y + ty;
            sf::Vector2f rayBack = sf::Vector2f((float) edgeX - player.player_position.x,
                                                (float) edgeY - player.player_position.y);
            float vecLength = GosRender::VectorLength(rayBack);
            float angleBetween = GosRender::AngleBetweenVectors(rayBack,
                                                                sf::Vector2f(dir.x, -dir.y));
            edges.push_back(std::make_pair(vecLength, angleBetween));
        }
    }
    std::sort(edges.begin(), edges.end(), [](const std::pair<float, float> &left,
                                             const std::pair<float, float> &right) {
        return left.first < right.first;
    });

    if (edges[0].second < angle_diff || edges[1].second < angle_diff)return true;
    return false;
};


static float GetTexturePositionColumn(int box_x, int box_y, float test_x, float test_y) {
    float pos = 0;
    sf::Vector2f blockCenter((float) box_x + 0.5f, (float) box_y + 0.5f);

    float testAngle = atan2f(test_y - blockCenter.y, test_x - blockCenter.x);

    if ((testAngle >= GosRender::PI / 4.f && testAngle <= (3.f / 4.f) * GosRender::PI)) {
        pos = test_x - static_cast<int>(test_x);
    } else if ((testAngle <= -GosRender::PI / 4.f &&
                testAngle >= -GosRender::PI * (3.f / 4.f))) {
        pos = test_x - static_cast<int>(test_x);
    } else if (testAngle <= GosRender::PI / 4.f && testAngle >= -GosRender::PI / 4.f) {
        pos = test_y - static_cast<int>(test_y);
    } else if (testAngle >= (3.f / 4.f) * GosRender::PI ||
               testAngle <= -(3.f / 4.f) * GosRender::PI) {
        pos = test_y - static_cast<int>(test_y);
    }
    return pos;
}


GosRender::RayResponse CastRay(GosRender::Player &player, float ray_angle, GosRender::Map map) {
    GosRender::RayResponse response;
    float dir_x = cosf(ray_angle), dir_y = sinf(ray_angle);
    float unit_step_x = abs(1.f / dir_x), unit_step_y = abs(1.f / dir_y);
    float x_change_distance = 0, y_change_distance = 0;
    float distance_to_wall = 0;
    int curr_box_x = static_cast<int>(player.player_position.x);
    int curr_box_y = static_cast<int>(player.player_position.y);
    int dx, dy;

    if (dir_x >= 0) {
        dx = 1;
        float xOffset = 1 - (player.player_position.x - (float) curr_box_x);
        x_change_distance = xOffset * unit_step_x;
    } else {
        dx = -1;
        float xOffset = player.player_position.x - (float) curr_box_x;
        x_change_distance = xOffset * unit_step_x;
    }

    if (dir_y >= 0) {
        dy = -1;
        float yOffset = player.player_position.y - (float) curr_box_y;
        y_change_distance = yOffset * unit_step_y;
    } else {
        dy = 1;
        float yOffset = 1 - (player.player_position.y - (float) curr_box_y);
        y_change_distance = yOffset * unit_step_y;
    }

    while (distance_to_wall < map.map_width) {
        if (x_change_distance < y_change_distance) {
            curr_box_x += dx;
            distance_to_wall = x_change_distance;
            x_change_distance += unit_step_x;
        } else {
            curr_box_y += dy;
            distance_to_wall = y_change_distance;
            y_change_distance += unit_step_y;
        }

        if (curr_box_x >= 0 && curr_box_x < map.map_width && curr_box_y >= 0 &&
            curr_box_y < map.map_height) {
            if (map.map[(curr_box_y * map.map_width) + curr_box_x] == '1') {
                response.distance = distance_to_wall;
                float test_x = (player.player_position.x + dir_x * distance_to_wall),
                        test_y = (player.player_position.y - dir_y * distance_to_wall);
                response.is_edge = IsEdge(player, curr_box_x, curr_box_y, sf::Vector2f(dir_x, dir_y));
                response.collision_position = GetTexturePositionColumn(curr_box_x, curr_box_y, test_x, test_y);
                return response;
            }
        }

    }
    return response;
}

static void LoadTextures() {
    wall_texture.loadFromFile("../resources/wall_texture.jpeg");

    floor_texture.loadFromFile("../resources/wall_texture.jpeg");

    ceilling_texture.loadFromFile("../resources/wall_texture.jpeg");
}

void GosRender::Render(GosRender::Player &player, GosRender::Map &map) {
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
    player.player_dir = sf::Vector2f(cosf(player.player_angle), sinf(player.player_angle));
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
            player.player_angle += 3 * delta_time;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            player.player_angle -= 3 * delta_time;
        }

        player.player_dir = sf::Vector2f(cosf(player.player_angle), sinf(player.player_angle));

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
                    player.player_position.x + static_cast<float>(cos(player.player_angle) * 4.5 * delta_time);

            float testCollisionY =
                    player.player_position.y - static_cast<float>(sin(player.player_angle) * 4.5 * delta_time);

            if (map.map[(int) testCollisionY * map.map_width + (int) testCollisionX] != '1') {
                player.player_position.x = testCollisionX;
                player.player_position.y = testCollisionY;
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            player.player_position.x -= static_cast<float>(cos(player.player_angle) * 4.5 * delta_time);
            player.player_position.y += static_cast<float>(sin(player.player_angle) * 4.5 * delta_time);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            player.player_height += 100 * delta_time;
        }

        for (int ray_column = 0; ray_column < GosRender::screen_width; ray_column++) {
            float ray_angle = (player.player_angle - (player.fieldOfView / 2.f) +
                               ((float) ray_column / (float) GosRender::screen_width) * player.fieldOfView);

            float dir_x = cosf(ray_angle);
            float dir_y = sinf(ray_angle);

            GosRender::RayResponse respo = CastRay(player, ray_angle, map);
            float distance_to_wall = respo.distance;
            bool is_edge = respo.is_edge;
            float pos = respo.collision_position;

            float fishEyeAngleFix = CosineOfAngleBetweenVectors(player.player_dir,
                                                                sf::Vector2f(dir_x, dir_y));
            distance_to_wall *= fishEyeAngleFix;

            float wall_height = ((player.distanceToProjectionPlane) / distance_to_wall) * GosRender::screen_height;

            float ceilling = (player.projection_center) - (wall_height / 2.f);

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




