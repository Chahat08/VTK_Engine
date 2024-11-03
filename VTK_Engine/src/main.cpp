// VTK_Engine.cpp : Defines the entry point for the application.
//

#include "app/app.h"
#include <iostream>

int main(int argc, char* argv[])
{
	if (argc < 7) 
		return -1;

	int sceneWidth = std::stoi(argv[1]);
	int sceneHeight = std::stoi(argv[2]);
	int instanceWidth = std::stoi(argv[3]);
	int instanceHeight = std::stoi(argv[4]);
	int xpos = std::stoi(argv[5]);
	int ypos = std::stoi(argv[6]);
	int physicalHeight = std::stof(argv[7]);
	int physicalDistance = std::stof(argv[8]);

	App::getInstance(
		sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos, physicalHeight, physicalDistance
	).run();

	return 0;
}
