#pragma once

#include "SFML/Graphics.hpp"
#include<iostream>
#include"math.h"


class Gos_Render {
private:

	//2d vector that describes direction of the player
	sf::Vector2f playerDir;

	//distance from player to camera
	float distanceToProjectionPlane;

	const int screenWdth;

	const int screenHght;

	//maximum distance from player to wall


	//delay time for per frame update
	float delay = 0.020f;


	std::string map;

	int mapWdth;

	int mapHght;

	float wallMiddle = screenHght / 2.0f;

	const float pi = 3.14159f;

	float playerX;

	float playerY;

	//angle where player is looking
	float playerAngle = 0.f;

	//FOV
	float fieldOfView = pi / 4.f;


	std::vector<sf::Sprite> texturedWalls;

	std::vector<sf::Sprite> texturedCeilling;

	//---------------------------------------

	std::vector<sf::RectangleShape> walls;

	std::vector<sf::RectangleShape> floorRects;

	std::vector<sf::RectangleShape> ceillingRects;

	std::vector<float> wallHeights;

	std::vector<float> distnces;
	//special sky with only stars
	std::vector<sf::CircleShape> stars;


	sf::Image img;

public:

	Gos_Render(int screenWidth, int screenHeight);

	void start();

	void setMap(std::string, int mapWdth, int mapHght);

	void setPosition(float x, float y);

	float angleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2);

	float vectorLength(sf::Vector2f vec);

	float cosineOfAngleBetweenVectors(sf::Vector2f vec1, sf::Vector2f vec2);

	sf::Vector2f norm(sf::Vector2f vec);

	void initFloor(std::vector<sf::RectangleShape>& floorRects, int clr);

	void initCeilling(std::vector<sf::RectangleShape>& ceillingRects, int clr);

	void makeDarkSky(std::vector<sf::CircleShape>& ceillingRects);

	void setDelay(const float delay);

	float getDelay(float delay) const;

	void initTexturedFloor(sf::Texture& floorTexture,sf::Texture & ceillingTexture);


};









































