#ifndef __WORLD_H__
#define __WORLD_H__
#include "Chunk.h"
#include <list>
#include <queue>
#include <thread>

class World
{
public:
    World(const glm::vec3& playerPos);
    void loadChunk(const glm::vec3 &position);
    void updateMeshes();

    void update();
    Chunk *getChunk(const glm::vec3 &position);

private:
    std::list<Chunk *> chunks;
    std::list<Chunk *> edgeChunks;

    std::vector<float> vOffsets;
    std::vector<glm::mat4> matrices;
};

#endif