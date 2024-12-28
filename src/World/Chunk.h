#ifndef __CHUNK_H__
#define __CHUNK_H__

#define CHUNK_X 16
#define CHUNK_Z 16
#define CHUNK_Y 256

#include "Block.h"
#include <glm/glm.hpp>
#include <vector>

class Chunk
{
public:
    Chunk(glm::vec3 position);

    std::vector<float> vOffset;
    std::vector<glm::mat4> matrices;
    glm::vec3 position;

    Chunk *left = nullptr;
    Chunk *right = nullptr;
    Chunk *front = nullptr;
    Chunk *back = nullptr;

private:
    Block blocks[CHUNK_Y][CHUNK_X][CHUNK_Z];

};

#endif