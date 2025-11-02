#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include "../Application/Application.h"

class Controller
{
public:
    static void KeyListen(GLFWwindow *window);
    static void MouseMove(double x, double y);
    static void MousePress(int button);
    static void WindowResize(int width, int height);
    static void scrollWheel(double xpos, double ypos);
};

#endif