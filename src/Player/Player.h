#ifndef __PLAYER_H__
#define __PLAYER_H__
#include "../Math/Camera.h"

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
        float height;
        float move_speed;
    };
    static Player &GetInstance();
    void init(const PlayerInfo &info);
    void move(int dir);
    void setFront(const glm::vec3 &front);
    inline Camera &getCamera() { return camera; }

private:
    Player();
    void updateCamera();
    
    Camera camera;
    PlayerInfo playerInfo;
};
#endif