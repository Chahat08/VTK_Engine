#ifndef WINDOW_H
#define WINDOW_H

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include "interaction/interactor.h"

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

	vtkRenderWindow* m_window{ nullptr };
	vtkRenderer* m_renderer{ nullptr };
	Interactor* m_interactor{ nullptr };

	void render();
};

#endif