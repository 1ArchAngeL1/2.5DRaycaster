#include<iostream>


#include "GraphicEngine.h"


int main() {
	Render render(720,1080);

	std::string map;

	map += "1111111111111111";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1..............1";
	map += "1111111111.11111";
	map += "1....1.........1";
	map += "1....1.........1";
	map += "1....1.........1";
	map += "1....1.........1";
	map += "1....1.........1";
	map += "1..............1";
	map += "1111111111111111";

	render.setMap(map,16,16);
	render.setPosition(8,8);
	render.start();

	std::cout << map[13] << std::endl;


	return 0;
}
