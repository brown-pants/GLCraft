#ifndef __WORLD_H__
#define __WORLD_H__
#include "Chunk.h"
#include <list>
#include <queue>
#include <mutex>
#include <thread>
#include <glm/gtc/matrix_transform.hpp>

class World
{
public:
    World();
    ~World();

    void init(const glm::vec3& playerPos, float sunAngle);

    float sunAngle() const { return sunRotateAngle; }

    void loadChunk(const glm::vec3 &position);
    Chunk *getChunk(const glm::vec3 &position);

    void updateRenderMeshes();

    bool digTest(const glm::vec3& pos);
    bool putTest(const glm::vec3& pos, const glm::vec3& dir, Block_Type block_type);
    bool touchTest(const glm::vec3& pos);

    static World* RunningWorld;

    void updatePlanet();

    glm::mat4 getSunModelMatrix() const { return glm::rotate(glm::mat4(1.0f), glm::radians(sunRotateAngle), glm::vec3(1.0f, 0.0f, 0.0f)); }
    glm::mat4 getMoonModelMatrix() const { return glm::rotate(glm::mat4(1.0f), glm::radians(sunRotateAngle + 180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); }

    glm::vec3 getSunPosition() const { return glm::vec3(getSunModelMatrix() * glm::vec4(0.0f, 0.0f, -100.0f, 1.0f)); }
    glm::vec3 getMoonPosition() const { return glm::vec3(getMoonModelMatrix() * glm::vec4(0.0f, 0.0f, -100.0f, 1.0f)); }

    glm::vec3 getSkyColor() const;

    void stop();

private:
    void updateMeshes();
    void update();

    std::list<Chunk *> chunks;
    std::list<Chunk *> edgeChunks;

    std::vector<float> vOffsets;
    std::vector<glm::mat4> matrices;
    
    std::thread* th_loadWorld;
    std::mutex mtx;

    bool m_running;
    bool isUpdateRenderMeshes;

    float sunRotateAngle;
};

#endif