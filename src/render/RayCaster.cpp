#include "RayCaster.h"
#include "2.5D/2.5DRender.h"


const int GosRender::screen_width = 1920;

const int GosRender::screen_height = 1080;

const int GosRender::map_width = 1080;

const int GosRender::map_height = 1080;

const sf::Vector2f GosRender::map_position(GosRender::screen_width * 0.5f - GosRender::map_width * 0.5f,
                                           GosRender::screen_height * 0.5f - GosRender::map_height * 0.5f);

void GosRender::Player::ChangleAnge(const float &dx) {
    this->angle += dx;
    this->player_dir = sf::Vector2f(cosf(this->angle), sinf(this->angle));
}

sf::Vector2f GosRender::Player::GetPosition() const {
    return this->position;
}

sf::Vector2f GosRender::Player::GetDirection() const {
    return this->player_dir;
}

float GosRender::Player::GetFieldOfView() const {
    return field_of_view;
}

float GosRender::Player::GetAngle() const {
    return this->angle;
}

void GosRender::Player::Move(sf::Vector2f direction, float speed, float delta_time) {
    this->position += direction * speed * delta_time;
}

void GosRender::Player::SetPosition(const sf::Vector2f &position) {
    this->position = position;
}

void GosRender::TopViewPlayer::SetPosition(const sf::Vector2f &position, const GosRender::Map &map) {
    this->position = position;
    this->UpdatePlayerPosition(map);
}

void GosRender::TopViewPlayer::DrawPlayer(sf::RenderWindow &window, const GosRender::Map &map) {
    this->UpdatePlayerPosition(map);
    window.draw(this->player_shape);
}

void GosRender::TopViewPlayer::UpdatePlayerPosition(const GosRender::Map &map) {
    float tile_world_height = GosRender::map_height / map.map_height;
    float tile_world_width = GosRender::map_width / map.map_width;
    this->player_shape.setPosition(
            GosRender::map_position + sf::Vector2f(position.x * tile_world_width, position.y * tile_world_height) -
            sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS));
}

void GosRender::TopViewPlayer::Move(sf::Vector2f direction, float speed, float delta_time, const GosRender::Map &map) {
    Player::Move(direction, speed, delta_time);
    this->UpdatePlayerPosition(map);
}

sf::Vector2f GosRender::TopViewPlayer::GetPositionOnRender(const GosRender::Map &map) {
    return this->player_shape.getPosition() + sf::Vector2f(PLAYER_RADIUS, PLAYER_RADIUS);
}

//topviewplayer

static bool IsEdge(GosRender::Player &player, int box_x, int box_y, sf::Vector2f dir) {
    float angle_diff = player.GetFieldOfView() / static_cast<float>(GosRender::screen_width);
    std::vector<std::pair<float, float >> edges;
    for (int dx = 0; dx < 2; dx++) {
        for (int dy = 0; dy < 2; dy++) {
            int edgeX = box_x + dx;
            int edgeY = box_y + dy;
            sf::Vector2f rayBack = sf::Vector2f((float) edgeX - player.GetPosition().x,
                                                (float) edgeY - player.GetPosition().y);
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


GosRender::RayResponse GosRender::CastRay(GosRender::Player *player, float ray_angle, GosRender::Map map) {
    GosRender::RayResponse response;
    float dir_x = cosf(ray_angle), dir_y = sinf(ray_angle);
    float unit_step_x = abs(1.f / dir_x), unit_step_y = abs(1.f / dir_y);
    float x_change_distance = 0, y_change_distance = 0;
    float distance_to_wall = 0;
    int curr_box_x = static_cast<int>(player->GetPosition().x);
    int curr_box_y = static_cast<int>(player->GetPosition().y);
    int dx, dy;

    if (dir_x >= 0) {
        dx = 1;
        float xOffset = 1 - (player->GetPosition().x - (float) curr_box_x);
        x_change_distance = xOffset * unit_step_x;
    } else {
        dx = -1;
        float xOffset = player->GetPosition().x - (float) curr_box_x;
        x_change_distance = xOffset * unit_step_x;
    }

    if (dir_y >= 0) {
        dy = -1;
        float yOffset = player->GetPosition().y - (float) curr_box_y;
        y_change_distance = yOffset * unit_step_y;
    } else {
        dy = 1;
        float yOffset = 1 - (player->GetPosition().y - (float) curr_box_y);
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
                float test_x = (player->GetPosition().x + dir_x * distance_to_wall),
                        test_y = (player->GetPosition().y - dir_y * distance_to_wall);
                response.is_edge = IsEdge(*player, curr_box_x, curr_box_y, sf::Vector2f(dir_x, dir_y));
                response.collision_position = GetTexturePositionColumn(curr_box_x, curr_box_y, test_x, test_y);
                return response;
            }
        }
    }
    return response;
}







