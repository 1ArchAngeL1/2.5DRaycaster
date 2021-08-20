#include "GraphicEngine.h"



float Render::angleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2) {
    float cosineOfAngle = ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (vectorLength(vec1) * vectorLength(vec2));
    return acos(cosineOfAngle);
}



float Render::vectorLength(sf::Vector2f vec) {
    return sqrt(pow(vec.x, 2.f) + pow(vec.y, 2.f));
}


float Render::cosineOfAngleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2) {
    return ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (vectorLength(vec1) * vectorLength(vec2));
}

void Render::start() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(Render::screenWdth,Render::screenHght), "2.5D",sf::Style::Default,settings);
    std::vector<sf::RectangleShape> walls;


    const float delay = 0.020f;
    sf::Clock clock;
    float chrono = 0;
    float maxDepth = (sqrt(Render::mapHght * Render::mapHght + Render::mapWdth * Render::mapWdth));
    Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        chrono += clock.getElapsedTime().asSeconds();
        clock.restart();
        Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));


        if (chrono >= delay) {
            
            walls.clear();
            chrono = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                Render::playerAngle += 0.1f;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                Render::playerAngle -= 0.1f;
               
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                float testCollisionX = playerX + cos(Render::playerAngle) * 0.2f;
                float testCollisionY = playerY - sin(Render::playerAngle) * 0.2f;
                if (Render::map[(int)testCollisionY * Render::mapWdth + (int)testCollisionX] != '1') {
                    Render::playerX = testCollisionX;
                    Render::playerY = testCollisionY;
                } 
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                Render::playerX -= cos(Render::playerAngle) * 0.2f;
                Render::playerY += sin(Render::playerAngle) * 0.2f;
            }
            


            for (int i = 0; i < Render::screenWdth; i++) {
                float rayAngle = (Render::playerAngle - (Render::fieldOfView / 2.f) + ((float)i / (float)Render::screenWdth) * Render::fieldOfView);
                float distanceToWall = 0;
                bool hitwall = false;
                
                float dirY = sinf(rayAngle);
                float dirX = cosf(rayAngle);

                float unitStepSizeX = abs(1.f / dirX);
                float unitStepSizeY = abs(1.f / dirY);
                
                int currBoxX = (int)Render::playerX;
                int currBoxY = (int)Render::playerY;

                float xChangeDistance = 0;
                float yChangeDistance = 0;

                int dx;
                int dy;

                if (dirX >= 0) {
                    dx = 1;
                    float xOffset = 1 - (Render::playerX - currBoxX);
                    xChangeDistance = xOffset * unitStepSizeX; 
                }
                else {
                    dx = -1;
                    float xOffset = Render::playerX - currBoxX;
                    xChangeDistance = xOffset  * unitStepSizeX;
                }

                if (dirY >= 0) {
                    dy = -1;
                    float yOffset = Render::playerY - currBoxY;
                    yChangeDistance = yOffset * unitStepSizeY;
                }
                else {
                    dy = 1;
                    float yOffset = 1 - (Render::playerY - currBoxY);
                    yChangeDistance = yOffset * unitStepSizeY;
                }

                while (!hitwall && distanceToWall < maxDepth) {
                    if (xChangeDistance < yChangeDistance) {
                        currBoxX += dx;
                        distanceToWall = xChangeDistance;
                        xChangeDistance += unitStepSizeX;
                    }
                    else {
                        currBoxY += dy;
                        distanceToWall = yChangeDistance;
                        yChangeDistance += unitStepSizeY;    
                    }

                    int testX = (int)(Render::playerX + dirX * distanceToWall);
                    int testY = (int)(Render::playerY + dirY * distanceToWall);


                    if (currBoxX >= 0 && currBoxX < Render::mapWdth && currBoxY >= 0 && currBoxY < Render::mapHght) {
                        if (Render::map[(currBoxY * Render::mapWdth) + currBoxX] == '1') {
                            hitwall = true;
                        }
                    }
                }

                float fishEyeAngleFix = Render::cosineOfAngleBetweenVectors(Render::playerDir, sf::Vector2f(dirX, dirY));

                distanceToWall *= fishEyeAngleFix;

                float ceilling = (float)(screenHght / 2.f) - (screenHght / (float)distanceToWall);
                float floor = screenHght - ceilling;
                sf::RectangleShape wall;
                wall.setSize(sf::Vector2f(1, floor - ceilling));
                wall.setPosition(sf::Vector2f(i, ceilling));
                wall.setFillColor(sf::Color::Yellow);
                walls.push_back(wall);
            }

        }

        window.clear();
        for (int i = 0; i < walls.size(); i++)window.draw(walls[i]);
        window.display();
    }
    return;
}


void Render::setPosition(float x, float y) {
    Render::playerX = x;
    Render::playerY = y;
}



void Render::setMap(std::string map, int mapWdth, int mapHght) {
    this->map = map;
    this->mapWdth = mapWdth;
    this->mapHght = mapHght;
}

