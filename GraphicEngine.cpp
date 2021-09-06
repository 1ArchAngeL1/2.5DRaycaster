#include "GraphicEngine.h"

#include "Functionals.h"


float Gos_Render::angleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2) {
    float cosineOfAngle = ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (vectorLength(vec1) * vectorLength(vec2));
    return acos(cosineOfAngle);
}


float Gos_Render::vectorLength(sf::Vector2f vec) {
    return sqrt(pow(vec.x, 2.f) + pow(vec.y, 2.f));
}


float Gos_Render::cosineOfAngleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2) {
    return ((vec1.x * vec2.x) + (vec1.y * vec2.y)) / (vectorLength(vec1) * vectorLength(vec2));
}


void Gos_Render::setPosition(float x, float y) {
    Gos_Render::playerX = x;
    Gos_Render::playerY = y;
}


void Gos_Render::setMap(std::string map, int mapWdth, int mapHght) {
    this->map = map;
    this->mapWdth = mapWdth;
    this->mapHght = mapHght;
}


sf::Vector2f Gos_Render::norm(sf::Vector2f vec) {
    float vecLen = Gos_Render::vectorLength(vec);
    return sf::Vector2f(vec.x / vecLen, vec.y / vecLen);
}


Gos_Render::Gos_Render(int screenWidth, int screenHeight) : screenWdth(screenHeight), screenHght(screenWidth) {
    Gos_Render::distanceToProjectionPlane = (0.5f) / tanf(Gos_Render::fieldOfView / 2.f);
    Gos_Render::img.create(Gos_Render::screenWdth, Gos_Render::screenHght, sf::Color::Black);
}



void Gos_Render::initFloor(std::vector<sf::RectangleShape> & floorRects, int clr) {
        for (int i = Gos_Render::screenHght / 2; i < (int)Gos_Render::screenHght; i += 3) {
            int color = clr * ((i - (Gos_Render::screenHght / 2.f)) / (Gos_Render::screenHght / 2.f));
            sf::RectangleShape shape;
            shape.setSize(sf::Vector2f(Gos_Render::screenWdth, 3));
            shape.setPosition(sf::Vector2f(0, i));
            shape.setFillColor(sf::Color(color, color, color));
            floorRects.push_back(shape);
        }  
}

void Gos_Render::initCeilling(std::vector<sf::RectangleShape>& ceillingRects, int clr) {
    for (int i =  0; i < Gos_Render::screenHght / 2; i += 3) {
        int color = clr - clr * (i / (Gos_Render::screenHght / 2.f));
        sf::RectangleShape shape;
        shape.setSize(sf::Vector2f(Gos_Render::screenWdth, 3));
        shape.setPosition(sf::Vector2f(0, i));
        shape.setFillColor(sf::Color(color, color, color));
        ceillingRects.push_back(shape);
    }
}



void Gos_Render::makeDarkSky(std::vector<sf::CircleShape>& ceillingRects) {
    for (int i = 0; i < Gos_Render::screenWdth; i+=5) {
        sf::CircleShape star(1);
        int starY = rand() % (Gos_Render::screenHght / 2);
        star.setFillColor(sf::Color::White);
        star.setPosition(i, starY);
        ceillingRects.push_back(star);
    }
}


void Gos_Render::setDelay(const float delay) {
    this->delay = delay;
}

float Gos_Render::getDelay(float delay) const {
    return this->delay;
}


sf::Vector2f operator*=(sf::Vector2f vec, int num) {
    return sf::Vector2f(vec.x * num, vec.y * num);
}

void Gos_Render::initTexturedFloor(sf::Texture & floorTexture,sf::Texture & ceillingTexture) {
    auto image = floorTexture.copyToImage();

    auto ceillingImage = ceillingTexture.copyToImage();
    
    for (int i = Gos_Render::wallMiddle; i < Gos_Render::screenHght; i++) {

        float straightDistanceToPoint = Gos_Render::distanceToProjectionPlane * ((Gos_Render::screenHght / 2.f) / (i - (Gos_Render::screenHght / 2.f)));

        sf::Vector2f leftVector(cosf(playerAngle - (Gos_Render::fieldOfView / 2.f)),sinf(playerAngle - (Gos_Render::fieldOfView / 2.f)));

        sf::Vector2f rightVector(cosf(playerAngle + (Gos_Render::fieldOfView / 2.f)), sinf(playerAngle + (Gos_Render::fieldOfView / 2.f)));

        float stepX = straightDistanceToPoint * (rightVector.x - leftVector.x) / Gos_Render::screenWdth;

        float stepY = straightDistanceToPoint * (rightVector.y - leftVector.y) / Gos_Render::screenWdth;
        
        float floorX = Gos_Render::playerX + straightDistanceToPoint * leftVector.x;

        float floorY = Gos_Render::playerY - straightDistanceToPoint * leftVector.y;

        for (int j = 0; j < Gos_Render::screenWdth; j++) {
            
            int cellX = (int)floorX;
            int cellY = (int)floorY;
           
            int pixX = ((int)(image.getSize().x * abs(floorX - cellX)) % (image.getSize().x - 1));
            int pixY = ((int)(image.getSize().y * abs(floorY - cellY)) % (image.getSize().y - 1));
                

            floorX += stepX;
            floorY -= stepY;


            Gos_Render::img.setPixel(j, i, image.getPixel(pixX, pixY));
            Gos_Render::img.setPixel(j,Gos_Render::screenHght - i,ceillingImage.getPixel(pixX, pixY));
        }
    }
   
}

void Gos_Render::start() {


    float maxDepth = (sqrt(Gos_Render::mapHght * Gos_Render::mapHght + Gos_Render::mapWdth * Gos_Render::mapWdth));

   

    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    sf::RenderWindow window(sf::VideoMode(Gos_Render::screenWdth, Gos_Render::screenHght),"2.5D",sf::Style::Default,settings);
   
    window.setFramerateLimit(144);
   
    std::vector<float> distanceToWalls;

    sf::Texture wallTexture;
    wallTexture.loadFromFile("modernwall.jfif");
    

    sf::Texture floorTexture;
    floorTexture.loadFromFile("modernfloor.jfif");

    sf::Texture ceillingTexture;
    ceillingTexture.loadFromFile("modernfloor.jfif");

    //Render::makeDarkSky(stars);

    
   /* Gos_Render::initCeilling(Gos_Render::ceillingRects, 170);
    Gos_Render::initFloor(Gos_Render::floorRects, 170);*/
   

    sf::Clock clock;

    float chrono = 0;


    Gos_Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));

    float angleDiff = Gos_Render::fieldOfView / ((float)Gos_Render::screenWdth);


    
    while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float deltaT = clock.getElapsedTime().asSeconds();

        chrono += deltaT;

        clock.restart();


        if (chrono >= Gos_Render::delay) {

            Gos_Render::texturedWalls.clear();

            
   
            chrono = 0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                Gos_Render::playerAngle += 0.1f;
                Gos_Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));
             

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                Gos_Render::playerAngle -= 0.1f;
                Gos_Render::playerDir = sf::Vector2f(cosf(playerAngle), sinf(playerAngle));
              
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                Gos_Render::wallMiddle -= 5.f;

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                
                Gos_Render::wallMiddle += 5.f;

            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
                float testCollisionX = Gos_Render::playerX + cos(Gos_Render::playerAngle) * 0.15f;
                float testCollisionY = Gos_Render::playerY - sin(Gos_Render::playerAngle) * 0.15f;
                if (Gos_Render::map[(int)testCollisionY * Gos_Render::mapWdth + (int)testCollisionX] != '1') {
                    Gos_Render::playerX = testCollisionX;
                    Gos_Render::playerY = testCollisionY;
                   
                } 

            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                Gos_Render::playerX -= cos(Gos_Render::playerAngle) * 0.2f;
                Gos_Render::playerY += sin(Gos_Render::playerAngle) * 0.2f;
              
            }
            


            for (int i = 0; i < Gos_Render::screenWdth; i++) {
                float rayAngle = (Gos_Render::playerAngle - (Gos_Render::fieldOfView / 2.f) + ((float)i / (float)Gos_Render::screenWdth) * Gos_Render::fieldOfView);
                float distanceToWall = 0;

                float pos = 0;

                bool hitwall = false;
                bool isEdge = false;
                
                float dirY = sinf(rayAngle);
                float dirX = cosf(rayAngle);

                float unitStepSizeX = abs(1.f / dirX);
                float unitStepSizeY = abs(1.f / dirY);
                
                int currBoxX = (int)Gos_Render::playerX;
                int currBoxY = (int)Gos_Render::playerY;

                float xChangeDistance = 0;
                float yChangeDistance = 0;

                int dx;
                int dy;

                if (dirX >= 0) {
                    dx = 1;
                    float xOffset = 1 - (Gos_Render::playerX - (float)currBoxX);
                    xChangeDistance = xOffset * unitStepSizeX; 
                }
                else {
                    dx = -1;
                    float xOffset = Gos_Render::playerX - (float)currBoxX;
                    xChangeDistance = xOffset  * unitStepSizeX;
                }

                if (dirY >= 0) {
                    dy = -1;
                    float yOffset = Gos_Render::playerY - (float)currBoxY;
                    yChangeDistance = yOffset * unitStepSizeY;
                }
                else {
                    dy = 1;
                    float yOffset = 1 - (Gos_Render::playerY - (float)currBoxY);
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


                    if (currBoxX >= 0 && currBoxX < Gos_Render::mapWdth && currBoxY >= 0 && currBoxY < Gos_Render::mapHght) {
                        if (Gos_Render::map[(currBoxY * Gos_Render::mapWdth) + currBoxX] == '1') {
                            hitwall = true;


                            float testX = (Gos_Render::playerX + dirX * distanceToWall);
                            float testY = (Gos_Render::playerY - dirY * distanceToWall);

                           
                            /*std::vector<std::pair<float ,float >> edges;
                            
                            for (int tx = 0; tx < 2; tx++) {
                                for (int ty = 0; ty < 2; ty++) {
                                    int edgeX = (int)currBoxX + tx;
                                    int edgeY = (int)currBoxY + ty;
                                    sf::Vector2f rayBack = sf::Vector2f((float)edgeX - Gos_Render::playerX,(float)edgeY - Gos_Render::playerY);
                                    float vecLength = Gos_Render::vectorLength(rayBack);
                                    float angleBetween = Gos_Render::angleBetweenVectors(rayBack,sf::Vector2f(dirX, -dirY));
                                    edges.push_back(std::make_pair(vecLength, angleBetween));
                                }
                            }

                            std::sort(edges.begin(), edges.end(), [](const std::pair<float, float>& left, const std::pair<float, float>& right) {return left.first < right.first;});

                            if (edges[0].second < angleDiff || edges[1].second < angleDiff)isEdge = true;*/

                            sf::Vector2f blockCenter((float)currBoxX + 0.5f, (float)currBoxY + 0.5f);

                            sf::Vector2f forAngle(testX - blockCenter.x, testY - blockCenter.y);
   
                            float testAngle = atan2f(testY - blockCenter.y, testX - blockCenter.x);


                            if ((testAngle >= Gos_Render::pi / 4.f && testAngle <= (3.f / 4.f) * Gos_Render::pi)){
                                pos = testX - (int)testX;
                            }
                            else if ((testAngle <= -Gos_Render::pi / 4.f && testAngle >= -Gos_Render::pi * (3.f / 4.f))) {
                                pos = testX - (int)testX;
                            }
                            else if (testAngle <= Gos_Render::pi / 4.f && testAngle >= -Gos_Render::pi / 4.f) {
                                pos = testY - (int)testY;
                            }
                            else if (testAngle >= (3.f / 4.f) * Gos_Render::pi || testAngle <= -(3.f / 4.f) * Gos_Render::pi) {
                                pos = testY - (int)testY;
                            }
                            
                        }
                    }


                }
               // Gos_Render::initTexturedFloor(Gos_Render::floorRects, floorTexture);

                distanceToWalls.push_back(distanceToWall);


                float fishEyeAngleFix = Gos_Render::cosineOfAngleBetweenVectors(Gos_Render::playerDir, sf::Vector2f(dirX, dirY));

              
                distanceToWall *= fishEyeAngleFix;


                float wallHeight = ((Gos_Render::distanceToProjectionPlane) / distanceToWall) * Gos_Render::screenHght;


                //float ceilling = (float)(Gos_Render::screenHght / 2.f) - (Gos_Render::screenHght / (float)distanceToWall);
                
                float ceilling = (Gos_Render::wallMiddle) - (wallHeight / 2.f);

                float floor = screenHght - ceilling;


              /*  sf::RectangleShape wall;
                wall.setSize(sf::Vector2f(1,wallHeight));
                wall.setPosition(sf::Vector2f(i, ceilling));

                 if (isEdge) {
                     wall.setFillColor(sf::Color::Black);
                 }
                 else {
                     int color = 170 - (distanceToWall / (float)Gos_Render::mapWdth) * 170;
                     wall.setFillColor(sf::Color(252 - (distanceToWall / (float)Gos_Render::mapWdth) * 252,
                         247 - (distanceToWall / (float)Gos_Render::mapWdth) * 247,
                         135 - (distanceToWall / (float)Gos_Render::mapWdth) * 135));
                 }
                 Gos_Render::walls.push_back(wall);*/


                sf::Sprite wallSprite;
                wallSprite.setTexture(wallTexture);
                wallSprite.setTextureRect(sf::IntRect(pos * 1024, 0, 1, 1024));
                wallSprite.setScale(sf::Vector2f(1, (wallHeight) / 1024.f));
                wallSprite.setPosition(sf::Vector2f(i, ceilling));

                int color = 256 - (distanceToWall / (float)Gos_Render::mapWdth) * 256;
                wallSprite.setColor(sf::Color(color, color, color));
                texturedWalls.push_back(wallSprite);
             
            }

           
            Gos_Render::initTexturedFloor(floorTexture,ceillingTexture);
        }

       

        sf::Texture floorBack;
        floorBack.loadFromImage(Gos_Render::img);
        sf::Sprite floorSprite;
        floorSprite.setTexture(floorBack);
        floorSprite.setPosition(sf::Vector2f(0, 0));
       
        window.clear();
        window.draw(floorSprite);
        for (int i = 0; i < Gos_Render::texturedWalls.size(); i++)window.draw(Gos_Render::texturedWalls[i]);
        window.display();
    }
    return;
}




