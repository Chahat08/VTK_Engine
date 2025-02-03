// main.cpp : Defines the entry point for the application.
//

#include "app/app.h"
#include <iostream>

int main(int argc, char* argv[])
{
    if (argc < 18)
        return -1;

    int sceneWidth = std::stoi(argv[1]);
    int sceneHeight = std::stoi(argv[2]);
    int instanceWidth = std::stoi(argv[3]);
    int instanceHeight = std::stoi(argv[4]);
    int xpos = std::stoi(argv[5]);
    int ypos = std::stoi(argv[6]);
    int windowXPos = std::stoi(argv[7]);
    int windowYPos = std::stoi(argv[8]);
    float physicalHeight = std::stof(argv[9]);
    float physicalDistance = std::stof(argv[10]);
    float angleToRotate = std::stof(argv[11]);
    int columnNumber = std::stoi(argv[12]);
    std::string clientID(argv[13]);
    std::string url(argv[14]);
    bool isHeadNode = std::stoi(argv[15]);
    int gpuIndex = std::stoi(argv[16]);
	bool showConsole = std::stoi(argv[17]);

    App::getInstance(
        sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos, windowXPos, windowYPos, physicalHeight, physicalDistance,
        angleToRotate * columnNumber,
        clientID, url, isHeadNode, gpuIndex
    ).run();

    return 0;
}
