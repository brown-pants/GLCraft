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
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glViewport(0, 0, width, height);
    glfwSetWindowUserPointer(m_window, &m_events);
    glEnable(GL_DEPTH_TEST);
}

Window::~Window()
{
    glfwTerminate();
}

void Window::update() const
{
    glfwSwapBuffers(m_window);
    glClearColor(0.1f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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