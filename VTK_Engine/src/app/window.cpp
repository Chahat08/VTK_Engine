#include "app/window.h"

Window::Window(int width, int height, int xpos, int ypos, int gpuIndex, bool borders, std::string name) {
#ifdef _WIN32	
	m_window = vtkWin32OpenGLRenderWindow::New();
#else
	m_window = vtkRenderWindow::New();
#endif
	m_window->SetSize(width, height);
	m_window->SetPosition(xpos, ypos);
	m_window->SetBorders(borders);
	m_window->SetWindowName(name.c_str());

	m_window->SetDeviceIndex(gpuIndex);

	m_renderer = vtkRenderer::New();
	m_window->AddRenderer(m_renderer);
}

Window::~Window() {
	m_window->Delete();
}

void Window::render() {
	std::cout << "Re rendering\n";
	m_window->Render();
}