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

    void init(const glm::vec3& playerPos, float sunAngle, const std::vector<std::queue<glm::vec3>> &flowingWater);

    void loadChunk(const glm::vec3 &position);
    Chunk *getChunk(const glm::vec3 &position);

    void updateRenderMeshes();

    bool digTest(const glm::vec3& pos);
    bool touchTest(const glm::vec3& pos);
    bool putTest(const glm::vec3& pos, const glm::vec3& dir, Block_Type block_type);
    bool physicalTest(const glm::vec3 pos, float height);

    static World* RunningWorld;

    void updatePlanet();

    inline glm::mat4 getSunModelMatrix() const { return glm::rotate(glm::mat4(1.0f), glm::radians(sunRotateAngle), glm::vec3(1.0f, 0.0f, 0.0f)); }
    inline glm::mat4 getMoonModelMatrix() const { return glm::rotate(glm::mat4(1.0f), glm::radians(sunRotateAngle + 180.0f), glm::vec3(1.0f, 0.0f, 0.0f)); }

    inline glm::vec3 getSunPosition() const { return glm::vec3(getSunModelMatrix() * glm::vec4(0.0f, 0.0f, -100.0f, 1.0f)); }
    inline glm::vec3 getMoonPosition() const { return glm::vec3(getMoonModelMatrix() * glm::vec4(0.0f, 0.0f, -100.0f, 1.0f)); }

    inline float &sunAngle() { return sunRotateAngle; }
    inline const std::vector<std::queue<glm::vec3>> &getFlowingWater() const { return m_flowingWater; }

    glm::vec3 getSkyColor() const;

    void stop();

    void updateFlowWater();
    void addFlowWater(const glm::vec3 &global_pos);

    int getBlockFromPosition(const glm::vec3 &pos);

private:
    void updateMeshes();
    void update();
    bool setWater(const glm::vec3 &pos);

    std::list<Chunk *> chunks;
    std::list<Chunk *> edgeChunks;

    std::vector<float> block_vOffsets;
    std::vector<glm::mat4> block_matrices;
    std::vector<glm::mat4> water_matrices;
    
    std::thread* th_loadWorld;
    std::mutex mtx;

    bool m_running;
    bool isUpdateRenderMeshes;

    float sunRotateAngle;
    std::vector<std::queue<glm::vec3>> m_flowingWater;
};

#endif