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
        float dropSpeed = 0.0f;
        float width = 0.35f;
        float jump = -0.12f;
        bool physical = true;
        bool multipleJump = false;
    };
    static Player &GetInstance();
    void init(const PlayerInfo &info);
    void move(int dir);
    void setFront(const glm::vec3 &front);
    inline Camera &getCamera() { return camera; }
    inline glm::vec3 getPosition() const { return playerInfo.position; }
    inline float getHeight() const { return playerInfo.height; }
    inline PlayerInfo &getInfo() { return playerInfo; }

    void dig();
    void put(Block_Type block_type);
    void jump();
    void physical();
    bool isBindPhysical() const { return playerInfo.physical; }
    void bindPhysical(bool sign) { playerInfo.physical = sign; }
    bool obstacleTest(const glm::vec3 &testPos);

private:
    Player();
    void updateCamera();
    
    Camera camera;
    PlayerInfo playerInfo;

    bool isLanding = true;
};
#endif