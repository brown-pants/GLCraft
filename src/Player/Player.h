#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "../Math/Camera.h"
#include "../World/Block.h"

class Player
{
public:
    enum MoveDirection
    {
        Front = 1,
        Back = 2,
        Left = 4,
        Right = 8
    };
    struct PlayerInfo
    {
        glm::vec3 position;
        glm::vec3 front;
        float height = 1.8f;
        float move_speed = 0.1f;
        float gravity = 0.004f;
        float dropSpeed = gravity;
        float width = 0.35f;
        float jump = -0.12f;
        bool physical = true;
        bool multipleJump = false;
        bool landing = true;
    };
    static Player &GetInstance();
    void init(const PlayerInfo &info);
    void move(int dir);
    void setFront(const glm::vec3 &front);
    inline Camera &getCamera() { return camera; }
    inline PlayerInfo &getInfo() { return info; }

    void dig();
    void put(Block_Type block_type);
    void jump();
    void physical();
    bool obstacleTest(const glm::vec3 &testPos);
    bool isDive();
    bool isSwim();

private:
    Player();
    void updateCamera();
    
    Camera camera;
    PlayerInfo info;
};
#endif