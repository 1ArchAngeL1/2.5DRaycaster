#pragma once

#include "SFML/Graphics.hpp"
#include<iostream>
#include"math.h"


class Render {
private:
	sf::Vector2f playerDir;

	int screenWdth;
	int screenHght;

	std::string map;

	int mapWdth;
	int mapHght;

	const float pi = 3.14159f;

	float playerX = 0.f;
	float playerY = 0.f;
	float playerAngle = 0.f;
	float fieldOfView = pi / 4.f;


public:

	Render(int screenWidth, int screenHeight): screenWdth(screenHeight), screenHght(screenWidth) {}
	void start();
	void setMap(std::string, int mapWdth, int mapHght);
	void setPosition(float x, float y);
	float angleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2);
	float vectorLength(sf::Vector2f vec);
	float cosineOfAngleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2);
	sf::Vector2f norm(sf::Vector2f vec);
};









































