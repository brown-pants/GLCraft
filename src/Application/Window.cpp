#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const char* title)
{
    glfwInit();
    m_window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(m_window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(m_window, &m_events);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::hide()
{
    glfwHideWindow(m_window);
}

void Window::show()
{
    glfwShowWindow(m_window);
}

void Window::close()
{
    glfwSetWindowShouldClose(m_window, 1);
}

int Window::width()
{
    int width;
    glfwGetWindowSize(m_window, &width, NULL);
    return width;
}

int Window::height()
{
    int height;
    glfwGetWindowSize(m_window, NULL, &height);
    return height;
}

void Window::update() const
{
    glfwSwapBuffers(m_window);
    glfwPollEvents();
}
bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::setMouseMoveEvent(const std::function<void(double, double)>& f)
{
    m_events.mouseMove = f;
    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xposIn, double yposIn) {
        ((Events*)glfwGetWindowUserPointer(window))->mouseMove(xposIn, yposIn);
    });
}

void Window::setWindowResizeEvent(const std::function<void(int, int)>& f)
{
    m_events.windowResize = f;
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        ((Events*)glfwGetWindowUserPointer(window))->windowResize(width, height);
        glViewport(0, 0, width, height);
    });
}

void Window::setMousePressEvent(const std::function<void(int, int)>& f)
{
    m_events.mousePress = f;
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        ((Events*)glfwGetWindowUserPointer(window))->mousePress(button, action);
        });
}

void Window::setScrollEvent(const std::function<void(double, double)>& f)
{
    m_events.scrollWheel = f;
    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xpos, double ypos){
        ((Events*)glfwGetWindowUserPointer(window))->scrollWheel(xpos, ypos);
    });
}