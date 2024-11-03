#ifndef WINDOW_H
#define WINDOW_H

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <string>

class Window {
public:
	Window() = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

protected:
	Window(int width, int height, int xpos, int ypos, bool borders = false, std::string name = "Title");
	~Window();

	vtkRenderWindow* m_window;
	vtkRenderer* m_renderer{nullptr};
	vtkRenderWindowInteractor* m_interactor{nullptr};

	void render();
};

#endif