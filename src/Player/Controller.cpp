#include "Controller.h"
#include "Player.h"
#include <iostream>
#include <ImGui/imgui.h>

bool is_cursor_disabled = false;

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

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        if (is_cursor_disabled)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            is_cursor_disabled = false;
        }
    }

    if (Player::GetInstance().isSwim() && (!Player::GetInstance().isLanding() || Player::GetInstance().isDive()))
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            Player::GetInstance().getInfo().dropSpeed = -0.03f;
        }
        else
        {
            Player::GetInstance().getInfo().dropSpeed = 0.03f;
        }
    }
    else
    {
        static bool spaceRelease = true;
        if (spaceRelease && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) 
        {
            Player::GetInstance().jump();
            spaceRelease = false;
        }
        else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) 
        {
            spaceRelease = true;
        }
    }
}

static bool firstMouse = true;
void Controller::MouseMove(double x, double y)
{
    if(!is_cursor_disabled)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddMousePosEvent((float)x, (float)y);
        return;
    }
    float xpos = static_cast<float>(x);
    float ypos = static_cast<float>(y);
    static float lastX, lastY, yaw, pitch;
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        glm::vec3 fornt = Player::GetInstance().getCamera().front;
        pitch = glm::degrees(asin(fornt.y));
        yaw = glm::degrees(atan2(fornt.z, fornt.x));
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

static int curBlock = ClayBlock;
void Controller::MousePress(int button, int action)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddMouseButtonEvent(button, action == GLFW_PRESS);
    if (io.WantCaptureMouse) 
    {
        return;
    }
    if (!is_cursor_disabled)
    {
        glfwSetInputMode(Application::GetApp()->getWindow()->getGlfwWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        is_cursor_disabled = true;
        firstMouse = true;
        return;
    }
    if (action != GLFW_PRESS)
    {
        return;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        Player::GetInstance().dig();
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        Player::GetInstance().put((Block_Type)curBlock);
    }
}

void Controller::WindowResize(int width, int height)
{
    if (height == 0)
    {
        return;
    }
    Player::GetInstance().getCamera().aspect = (float)width / (float)height;
}

void Controller::scrollWheel(double xpos, double ypos)
{
    if (ypos > 0)
    {
        curBlock += 1;
    }
    else
    {
        curBlock -= 1;
    }
    if (curBlock >= BLOCK_TYPE_NUM)
    {
        curBlock = 2;
    }
    else if (curBlock <= 1)
    {
        curBlock = 8;
    }
}

std::string Controller::getCurBlock()
{
    static std::string blockNames[] = 
    {
        "Air",
        "Water",
        "ClayBlock",
        "GrassBlock",
        "StoneBlock",
        "SandBlock",
        "SnowBlock",
        "LogBlock",
        "LeaveBlock",
        "CactusBlock"
    };
    return blockNames[curBlock];
}