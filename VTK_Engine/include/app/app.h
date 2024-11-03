#ifndef APP_H
#define APP_H

#include "app/window.h"

class App :private Window {
public:
	static App& getInstance(int height, int width, int xpos, int ypos, bool borders = false, std::string name = "title");
	void run();

private:
	App(int width, int height, int xpos, int ypos, bool borders = false, std::string name = "title");
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;
};

#endif