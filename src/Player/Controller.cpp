#include "Controller.h"
#include "Player.h"
#include <iostream>

void Controller::KeyListen(GLFWwindow *window)
{
    int moveDir = 0;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
    {
        moveDir = moveDir | Player::Front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
    {
        moveDir = moveDir | Player::Back;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
    {
        moveDir = moveDir | Player::Left;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
    {
        moveDir = moveDir | Player::Right;
    }
    if(moveDir)
    {
        Player::GetInstance().move(moveDir);
    }
}

void Controller::MouseMove(double x, double y)
{
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);
    static float lastX, lastY, yaw = -90.0f, pitch = 0.0f;
    static bool firstMouse = true;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Player::GetInstance().setFront(glm::normalize(newFront));
}

void Controller::MousePress(int button)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        Player::GetInstance().dig();
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {

    }
}

void Controller::WindowResize(int width, int height)
{
    Player::GetInstance().getCamera().aspect = (float)width / (float)height;
}