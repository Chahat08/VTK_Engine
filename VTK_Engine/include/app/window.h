#ifndef WINDOW_H
#define WINDOW_H

#ifdef _WIN32
	#include <vtkWin32OpenGLRenderWindow.h>
#else
	#include <vtkRenderWindow.h>
#endif

#include <vtkRenderer.h>
#include <string>

class Window {
public:
	Window() = delete;
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;
	Window(Window&&) = delete;
	Window& operator=(Window&&) = delete;

protected:
	Window(int width, int height, int xpos, int ypos, int gpuIndex = 0, bool borders = false, std::string name = "Title");
	~Window();
#ifdef _WIN32
	vtkWin32OpenGLRenderWindow* m_window{ nullptr };
#else
	vtkRenderWindow* m_window{ nullptr };
#endif
	vtkRenderer* m_renderer{ nullptr };

	void render();
};

#endif