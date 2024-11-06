#include "app/window.h"

Window::Window(int width, int height, int xpos, int ypos, bool borders, std::string name) {
	m_window = vtkRenderWindow::New();
	m_window->SetSize(width, height);
	m_window->SetPosition(xpos, ypos);
	m_window->SetBorders(borders);
	m_window->SetWindowName(name.c_str());

	m_renderer = vtkRenderer::New();
	m_window->AddRenderer(m_renderer);
}

Window::~Window() {
	m_window->Delete();
}

void Window::render() {
	m_window->Render();
}