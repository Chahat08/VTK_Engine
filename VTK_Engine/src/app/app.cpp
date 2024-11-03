#include "app/app.h"

App::App(int width, int height, int xpos, int ypos, bool borders, std::string name) :Window(width, height, xpos, ypos, borders, name) {
	m_renderer = vtkRenderer::New();
}

void App::run() {
	render();
}

App& App::getInstance(int height, int width, int xpos, int ypos, bool borders, std::string name) {
	static App instance(height, width, xpos, ypos, borders, name);
	return instance;
}

App::~App() {
	m_renderer->Delete();
}