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
        float height;
        float move_speed;
    };
    static Player &GetInstance();
    void init(const PlayerInfo &info);
    void move(int dir);
    void setFront(const glm::vec3 &front);
    inline Camera &getCamera() { return camera; }
    inline glm::vec3 getPosition() const { return playerInfo.position; }

    void dig();
    void put(Block_Type block_type);

private:
    Player();
    void updateCamera();
    
    Camera camera;
    PlayerInfo playerInfo;
};
#endif