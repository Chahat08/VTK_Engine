﻿// main.cpp : Defines the entry point for the application.
//

#include "app/app.h"
#include <iostream>
#include <Windows.h>
#include <shellapi.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

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
    std::wstring wClientID(argv[13]);
    std::string clientID(wClientID.begin(), wClientID.end());
    std::wstring wUrl(argv[14]);
    std::string url(wUrl.begin(), wUrl.end());
    bool isHeadNode = std::stoi(argv[15]);
    int gpuIndex = std::stoi(argv[16]);
	bool showConsole = std::stoi(argv[17]);

    if (showConsole) {
        AllocConsole();
        FILE* fp;
        freopen_s(&fp, "CONOUT$", "w", stdout);
        freopen_s(&fp, "CONOUT$", "w", stderr);
        freopen_s(&fp, "CONIN$", "r", stdin);
        std::ios::sync_with_stdio(true);
    }

    App::getInstance(
        sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos, windowXPos, windowYPos, physicalHeight, physicalDistance,
        angleToRotate * columnNumber,
        clientID, url, isHeadNode, gpuIndex
    ).run();

    LocalFree(argv);
    return 0;
}
