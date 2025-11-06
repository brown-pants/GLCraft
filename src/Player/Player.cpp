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
    this->info = info;
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

    glm::vec3 front_dir = info.front;
    if (info.physical)
    {
        front_dir.y = 0.0f;
        front_dir = glm::normalize(front_dir);
    }

    glm::vec3 left_dir = glm::cross(camera.up, glm::vec3(info.front.x, 0.0f, info.front.z));
    left_dir = glm::normalize(left_dir);
    
    float speed = info.move_speed * 100 / Application::GetFps();
    if (speed >= 0.5f)
    {
        speed = 0.4f;
    }

    float front_distance = front;
    float left_distance = left;
    if(front && left)
    {
        front_distance *= sqrt(2) / 2.0;
        left_distance *= sqrt(2) / 2.0;
    }
    
    glm::vec3 front_move = front_dir * speed * front_distance;
    glm::vec3 left_move = left_dir * speed * left_distance;

    if (info.physical)
    {
        glm::vec3 testPos;
        // x test
        testPos = info.position;
        testPos.x += front_move.x + left_move.x;
        if (!obstacleTest(testPos))
        {
            info.position.x += front_move.x + left_move.x;
        }
        // z test
        testPos = info.position;
        testPos.z += front_move.z + left_move.z;
        if (!obstacleTest(testPos))
        {
            info.position.z += front_move.z + left_move.z;
        }
    }
    else
    {
        info.position += front_move;
        info.position += left_move;
    }
    
    updateCamera();
}

void Player::setFront(const glm::vec3 &front)
{
    info.front = front;
    updateCamera();
}

void Player::dig()
{
    float stride = 0.01f;
    float maxLength = 10.0f;
    float curLength = 0.0f;
    glm::vec3 pos = camera.position;
    const glm::vec3 dir = camera.front * stride;
    while (curLength <= maxLength)
    {
        pos += dir;
        if (World::RunningWorld->digTest(pos))
        {
            break;
        }
        curLength += stride;
    }
}

void Player::put(Block_Type block_type)
{
    float stride = 0.01f;
    float maxLength = 10.0f;
    float curLength = 0.0f;
    glm::vec3 pos = camera.position;
    const glm::vec3 dir = camera.front * stride;
    while (curLength <= maxLength)
    {
        pos += dir;
        if (World::RunningWorld->putTest(pos, dir, block_type))
        {
            break;
        }
        curLength += stride;
    }
}

void Player::jump()
{
    info.dropSpeed = info.jump;
}

void Player::updateCamera()
{
    camera.front = info.front;
    camera.position = glm::vec3(info.position.x, info.position.y + info.height - 0.2f, info.position.z);
}

void Player::physical()
{
    if (!info.physical)
    {
        return;
    }

    float dropSpeed = info.dropSpeed * 120 / Application::GetFps();
    if (dropSpeed >= 1.0f)
    {
        dropSpeed = 0.9f;
    }

    glm::vec3 testPos = info.position;
    testPos.y -= dropSpeed;
    
    if (obstacleTest(testPos))
    {
        if (info.dropSpeed >= 0.0f)
        {
            info.landing = true;
        }
        info.dropSpeed = info.gravity;
    }
    else
    {
        info.landing = false;
        info.position = testPos;
        info.dropSpeed += info.gravity;
        updateCamera();
    }

    if (info.dropSpeed >= 10000.0f)
    {
        info.dropSpeed = 10000.0f;
    }
}

bool Player::obstacleTest(const glm::vec3 &testPos)
{
    return
    World::RunningWorld->physicalTest(testPos + glm::vec3(info.width, 0, info.width), info.height) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(info.width, 0, -info.width), info.height) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(-info.width, 0, info.width), info.height) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(-info.width, 0, -info.width), info.height);
}

bool Player::isDive()
{
    if (camera.position.y > SEA_HORIZON + 1)
    {
        return false;
    }
    for (int y = info.position.y; y <= SEA_HORIZON; y ++)
    {
        int type = World::RunningWorld->getBlockFromPosition(glm::vec3(info.position.x, y, info.position.z));
        if (type == Water)
        {
            return true;
        }
        else if (type != Air)
        {
            return false;
        }
    }
    return false;
}

bool Player::isSwim()
{
    if (info.position.y + info.height / 2 > SEA_HORIZON + 1)
    {
        return false;
    }
    
    for (int y = info.position.y; y <= SEA_HORIZON; y ++)
    {
        int type = World::RunningWorld->getBlockFromPosition(glm::vec3(info.position.x, y, info.position.z));
        if (type == Water)
        {
            return true;
        }
        else if (type != Air)
        {
            return false;
        }
    }
    return false;
}