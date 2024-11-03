#ifndef APP_H
#define APP_H

#include "app/window.h"
#include "camera/camera.h"

class App :private Window {
public:
	static App& getInstance(
		int sceneWidth, int sceneHeight, 
		int instanceWidth, int instanceHeight,
		int xpos, int ypos, 
		float physicalHeight, float physicalDistance
	);
	void run();

private:
	App(int sceneWidth, int sceneHeight,
		int instanceWidth, int instanceHeight,
		int xpos, int ypos,
		float physicalHeight, float physicalDistance);
	~App();
	App(const App&) = delete;
	App& operator=(const App&) = delete;

	Camera* m_camera;
};

#endif