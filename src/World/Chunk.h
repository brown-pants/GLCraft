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
    Chunk(glm::vec3 position, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);
    ~Chunk();

    inline glm::vec3 getPosition() const { return position; }

    inline std::vector<float>& getVOffsets() { return vOffsets; }
    inline std::vector<glm::mat4>& getMatrices() { return matrices; }
    
    inline Chunk* getLeftChunk() const { return leftChunk; }
    inline Chunk* getRightChunk() const { return rightChunk; }
    inline Chunk* getFrontChunk() const { return frontChunk; }
    inline Chunk* getBackChunk() const { return backChunk; }

    void updateMesh();

    bool dig(int y, int x, int z);
    bool put(int y, int x, int z, Block_Type type);

private:
    void generateTree(int y, int x, int z, int height);
    void generateCactus(int y, int x, int z, int height);
    Block blocks[CHUNK_Y][CHUNK_X][CHUNK_Z];

    Chunk *leftChunk = nullptr;
    Chunk *rightChunk = nullptr;
    Chunk *frontChunk = nullptr;
    Chunk *backChunk = nullptr;

    std::vector<float> vOffsets;
    std::vector<glm::mat4> matrices;
    glm::vec3 position;
};

#endif