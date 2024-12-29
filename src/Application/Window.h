#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <functional>

class Window
{
public:
	Window(int width, int height, const char* title);
	~Window();

	void hide();
	void show();

	void update() const;
	bool shouldClose() const;
	void setMouseMoveEvent(const std::function<void(double, double)>& f);
	void setWindowResizeEvent(const std::function<void(int, int)>& f);
	void setMousePressEvent(const std::function<void(int)>& f);

	inline GLFWwindow* getGlfwWindow() const { return m_window; }

private:
	GLFWwindow* m_window;
	struct Events
	{
		std::function<void(double, double)> mouseMove;
		std::function<void(int, int)> windowResize;
		std::function<void(int)> mousePress;
	}m_events;
};
#endif