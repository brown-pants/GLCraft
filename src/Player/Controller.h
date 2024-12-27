#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__
#include <glfw/glfw3.h>

class Controller
{
public:
    static void KeyListen(GLFWwindow *window);
    static void MouseMove(double x, double y);
    static void WindowResize(int width, int height);
};

#endif