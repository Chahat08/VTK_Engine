#include "app/app.h"

App::App(int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) :Window(instanceWidth, instanceHeight, xpos, ypos) {
	m_renderer = vtkRenderer::New();
	m_camera = new Camera(sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos, physicalHeight, physicalDistance);
	m_renderer->SetActiveCamera(m_camera->getCamera());
}

void App::run() {
	render();
}

App& App::getInstance(
	int sceneWidth, int sceneHeight,
	int instanceWidth, int instanceHeight,
	int xpos, int ypos,
	float physicalHeight, float physicalDistance) {
	static App instance(sceneWidth, sceneHeight, instanceWidth, instanceHeight, xpos, ypos,	physicalHeight, physicalDistance);
	return instance;
}

App::~App() {
	m_renderer->Delete();
}