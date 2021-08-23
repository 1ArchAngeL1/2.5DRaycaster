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


void Render::setPosition(float x, float y) {
    Render::playerX = x;
    Render::playerY = y;
}


void Render::setMap(std::string map, int mapWdth, int mapHght) {
    this->map = map;
    this->mapWdth = mapWdth;
    this->mapHght = mapHght;
}


sf::Vector2f Render::norm(sf::Vector2f vec) {
    float vecLen = Render::vectorLength(vec);
    return sf::Vector2f(vec.x / vecLen, vec.y / vecLen);
}



void Render::start() {

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(Render::screenWdth,Render::screenHght), "2.5D",sf::Style::Default,settings);
    std::vector<sf::RectangleShape> walls;

    std::vector<sf::Sprite> texturedWalls;

    //512 x 512
    sf::Texture txt;
    txt.loadFromFile("texture.png");

    
    const float delay = 0.020f;
    sf::Clock clock;
    float chrono = 0;
    float maxDepth = (sqrt(Render::mapHght * Render::mapHght + Render::mapWdth * Render::mapWdth));
    Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));

    float angleDiff = Render::fieldOfView / (2.f *  (float)Render::screenWdth);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        float deltaT = clock.getElapsedTime().asSeconds();
        chrono += deltaT;
        clock.restart();


        if (chrono >= delay) {
            
            walls.clear();
            texturedWalls.clear();
            chrono = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
               // Render::playerAngle += 2.f * deltaT;
                Render::playerAngle += 0.1f;
                Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                //Render::playerAngle -= 2.f *  deltaT;
                Render::playerAngle -= 0.1f;
                Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));
               
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

                float pos = 0;

                bool hitwall = false;
                bool isEdge = false;
                
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


 
                    if (currBoxX >= 0 && currBoxX < Render::mapWdth && currBoxY >= 0 && currBoxY < Render::mapHght) {
                        if (Render::map[(currBoxY * Render::mapWdth) + currBoxX] == '1') {
                            hitwall = true;


                            float testX = (Render::playerX + dirX * distanceToWall);
                            float testY = (Render::playerY + dirY * distanceToWall);

                            int blockX = (int)(Render::playerX + dirX * distanceToWall);
                            int blockY = (int)(Render::playerY + dirY * distanceToWall);

                          /*  std::vector<std::pair<float ,float >> edges;*/
                            
                           /* for (int tx = 0; tx < 2; tx++) {
                                for (int ty = 0; ty > -2; ty--) {
                                    int edgeX = (int)testX + tx;
                                    int edgeY = (int)testY + ty;
                                    sf::Vector2f rayBack = sf::Vector2f(edgeX - Render::playerX,edgeY - Render::playerY);
                                    float vecLength = Render::vectorLength(rayBack);
                                    float angleBetween = Render::angleBetweenVectors(rayBack,sf::Vector2f(dirX, dirY));
                                    edges.push_back(std::make_pair(vecLength, angleBetween));
                                }
                            }

                            std::sort(edges.begin(), edges.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first; });

                            if (edges[0].second <= angleDiff || edges[1].second <= angleDiff)isEdge = true;*/


                            sf::Vector2f blockCenter((float)blockX + 0.5f, (float)blockY + 0.5f);

                            sf::Vector2f forAngle(testX - blockCenter.x, testY - blockCenter.y);


                            float testAngle = atan(forAngle.y / forAngle.x);

                            if ((testAngle >= Render::pi / 4.f && testAngle <= (3.f / 4.f) * Render::pi) || (testAngle < -Render::pi / 4.f && testAngle > -(3.f / 4.f) * Render::pi)) {
                                pos = testX - (float)blockX;
                            }
                            else {
                                pos = testY - (float)blockY;
                            }

                            
                        }
                    }

                  
                }

                float fishEyeAngleFix = Render::cosineOfAngleBetweenVectors(Render::playerDir, sf::Vector2f(dirX, dirY));


                distanceToWall *= fishEyeAngleFix;

                float ceilling = (float)(screenHght / 2.f) - (screenHght / (float)distanceToWall);
                float floor = screenHght - ceilling;
               /* sf::RectangleShape wall;
                wall.setSize(sf::Vector2f(1, floor - ceilling));
                wall.setPosition(sf::Vector2f(i, ceilling));*/

                sf::Sprite wallSprite;
                wallSprite.setTexture(txt);
                wallSprite.setTextureRect(sf::IntRect(pos * 512, 0, 1, 512));
                wallSprite.setScale(sf::Vector2f(1, (floor - ceilling) / 512.f));
                wallSprite.setPosition(sf::Vector2f(i, ceilling));

                int color = 256 - (distanceToWall / (float)Render::mapWdth) * 256;
                wallSprite.setColor(sf::Color(color, color, color));
                
                

                texturedWalls.push_back(wallSprite);
               
               /* if (isEdge) {
                    wall.setFillColor(sf::Color::Black);
                }
                else {
                    int color = 256 - (distanceToWall / (float)Render::mapWdth) * 256;
                    wall.setFillColor(sf::Color(color, color, color));
                }*/
               /* int color = 256 - (distanceToWall / (float)Render::mapWdth) * 256;
                walls.push_back(wall);*/
            }

        }

        window.clear();
        for (int i = 0; i < texturedWalls.size(); i++)window.draw(texturedWalls[i]);
        window.display();
    }
    return;
}




