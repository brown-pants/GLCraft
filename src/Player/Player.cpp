#include "Player.h"
#include "../Application/Application.h"
#include "../World/World.h"

Player &Player::GetInstance()
{
    static Player __instance;
    return __instance;
}

Player::Player()
{
}

void Player::init(const PlayerInfo &info)
{
    playerInfo = info;
    updateCamera();
}

void Player::move(int dir)
{
    int front = 0;
    int left = 0;
    if(dir & 1)
    {
        front ++;
    }
    if((dir >> 1) & 1)
    {
        front --;
    }
    if((dir >> 2) & 1)
    {
        left ++;
    }
    if((dir >> 3) & 1)
    {
        left --;
    }

    float speed = playerInfo.move_speed / Application::GetFps();
    glm::vec3 left_dir = glm::cross(camera.up, glm::vec3(playerInfo.front.x, 0.0f, playerInfo.front.z));

    float front_distance = front;
    float left_distance = left;
    if(front && left)
    {
        front_distance *= sqrt(2) / 2.0;
        left_distance *= sqrt(2) / 2.0;
    }
    
    playerInfo.position += playerInfo.front * speed * front_distance;
    playerInfo.position += left_dir * speed * left_distance;
    
    updateCamera();
}

void Player::setFront(const glm::vec3 &front)
{
    playerInfo.front = front;
    updateCamera();
}

void Player::dig()
{
    float stride = 0.01f;
    float maxLength = 10.0f;
    float curLenfth = 0.0f;
    glm::vec3 pos = camera.position;
    const glm::vec3 dir = camera.front * stride;
    while (curLenfth <= maxLength)
    {
        pos += dir;
        if (World::RunningWorld->digTest(pos))
        {
            break;
        }
        curLenfth += stride;
    }
}

void Player::updateCamera()
{
    camera.front = playerInfo.front;
    camera.position = glm::vec3(playerInfo.position.x, playerInfo.position.y + playerInfo.height, playerInfo.position.z);
}
