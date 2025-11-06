#ifndef __CHUNK_H__
#define __CHUNK_H__

#define CHUNK_X 16
#define CHUNK_Z 16
#define CHUNK_Y 256
#define SEA_HORIZON 120

#include "Block.h"
#include <glm/glm.hpp>
#include <vector>
#include <tuple>

class Chunk
{
public:
    Chunk(glm::vec3 position, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk);
    ~Chunk();

    inline glm::vec3 getPosition() const { return position; }

    inline std::vector<float>& getVOffsets() { return block_vOffsets; }
    inline std::vector<glm::mat4>& getMatrices() { return block_matrices; }
    inline std::vector<glm::mat4>& getWaterMatrices() { return water_matrices; }
    
    inline Chunk* getLeftChunk() const { return leftChunk; }
    inline Chunk* getRightChunk() const { return rightChunk; }
    inline Chunk* getFrontChunk() const { return frontChunk; }
    inline Chunk* getBackChunk() const { return backChunk; }

    void updateMesh();

    int getBlockType(int y, int x, int z);
    void setBlock(int y, int x, int z, Block_Type type);

    bool dig(int y, int x, int z);
    bool put(int y, int x, int z, Block_Type type);

private:
    void generateTree(int y, int x, int z, int height);
    void generateCactus(int y, int x, int z, int height);
    void loadBlock(int x, int y, int z, Block_Type block_type);
    bool aroundWater(int x, int y, int z);
    std::tuple<bool, bool, bool, bool, bool, bool> aroundTransparent(int x, int y, int z);
    
    glm::vec3 position;
    Block blocks[CHUNK_Y][CHUNK_X][CHUNK_Z];

    Chunk *leftChunk = nullptr;
    Chunk *rightChunk = nullptr;
    Chunk *frontChunk = nullptr;
    Chunk *backChunk = nullptr;

    std::vector<float> block_vOffsets;
    std::vector<glm::mat4> block_matrices;
    std::vector<glm::mat4> water_matrices;
};

#endif