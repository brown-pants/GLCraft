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

    glm::vec3 front_dir = playerInfo.front;
    if (playerInfo.physical)
    {
        front_dir.y = 0.0f;
        front_dir = glm::normalize(front_dir);
    }

    glm::vec3 left_dir = glm::cross(camera.up, glm::vec3(playerInfo.front.x, 0.0f, playerInfo.front.z));
    left_dir = glm::normalize(left_dir);
    
    float speed = playerInfo.move_speed * 100 / Application::GetFps();
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

    if (playerInfo.physical)
    {
        glm::vec3 testPos;
        // x test
        testPos = playerInfo.position;
        testPos.x += front_move.x + left_move.x;
        if (!obstacleTest(testPos))
        {
            playerInfo.position.x += front_move.x + left_move.x;
        }
        // z test
        testPos = playerInfo.position;
        testPos.z += front_move.z + left_move.z;
        if (!obstacleTest(testPos))
        {
            playerInfo.position.z += front_move.z + left_move.z;
        }
    }
    else
    {
        playerInfo.position += front_move;
        playerInfo.position += left_move;
    }
    
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
    if (playerInfo.multipleJump || m_isLanding)
    {
        playerInfo.dropSpeed = playerInfo.jump;
    }
}

void Player::updateCamera()
{
    camera.front = playerInfo.front;
    camera.position = glm::vec3(playerInfo.position.x, playerInfo.position.y + playerInfo.height - 0.2f, playerInfo.position.z);
}

void Player::physical()
{
    if (!playerInfo.physical)
    {
        return;
    }
    glm::vec3 testPos = playerInfo.position;
    testPos.y -= playerInfo.dropSpeed;
    if (obstacleTest(testPos))
    {
        if (playerInfo.dropSpeed >= 0.0f)
        {
            m_isLanding = true;
        }
        playerInfo.dropSpeed = playerInfo.gravity;
    }
    else
    {
        m_isLanding = false;
        playerInfo.position = testPos;
        playerInfo.dropSpeed += playerInfo.gravity;
        updateCamera();
    }
    if (playerInfo.dropSpeed >= 1.0f)
    {
        playerInfo.dropSpeed = 0.9f;
    }
}

bool Player::obstacleTest(const glm::vec3 &testPos)
{
    return
    World::RunningWorld->physicalTest(testPos + glm::vec3(playerInfo.width, 0, playerInfo.width)) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(playerInfo.width, 0, -playerInfo.width)) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(-playerInfo.width, 0, playerInfo.width)) ||
    World::RunningWorld->physicalTest(testPos + glm::vec3(-playerInfo.width, 0, -playerInfo.width));
}

bool Player::isDive()
{
    if (camera.position.y > SEA_HORIZON + 1)
    {
        return false;
    }
    const glm::vec3 &player_pos = playerInfo.position;
    if (World::RunningWorld->getBlockFromPosition(glm::vec3(player_pos.x, SEA_HORIZON, player_pos.z)) == Water)
    {
        return true;
    }
    return false;
}

bool Player::isSwim()
{
    if (playerInfo.position.y + playerInfo.height / 2 > SEA_HORIZON + 1)
    {
        return false;
    }
    const glm::vec3 &player_pos = playerInfo.position;
    if (World::RunningWorld->getBlockFromPosition(glm::vec3(player_pos.x, SEA_HORIZON, player_pos.z)) == Water)
    {
        return true;
    }
    return false;
}