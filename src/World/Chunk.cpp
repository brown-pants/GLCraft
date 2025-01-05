#include "Chunk.h"
#include "../Application/Application.h"
#include "../Math/Noise/PerlinNoise.h"
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 Rotate_left = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_right = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_top = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 Rotate_bottom = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 Rotate_front = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_back = glm::mat4(1.0f);

Chunk::Chunk(glm::vec3 position, Chunk* leftChunk, Chunk* rightChunk, Chunk* frontChunk, Chunk* backChunk) : position(position)
{
    if (leftChunk != nullptr)
    {
        this->leftChunk = leftChunk;
        leftChunk->rightChunk = this;
    }
    if (rightChunk != nullptr)
    {
        this->rightChunk = rightChunk;
        rightChunk->leftChunk = this;
    }
    if (frontChunk != nullptr)
    {
        this->frontChunk = frontChunk;
        frontChunk->backChunk = this;
    }
    if (backChunk != nullptr)
    {
        this->backChunk = backChunk;
        backChunk->frontChunk = this;
    }
    for(int x = 0; x < CHUNK_X; x ++)
    {
        for(int z = 0; z < CHUNK_Z; z ++)
        {
            int max_y = 
                PerlinNoise::Noise_2d(0.2, 64, position.x + x, position.z + z) +
                PerlinNoise::Noise_2d(0.8, 32, position.x + x, position.z + z) +
                PerlinNoise::Noise_2d(1.6, 16, position.x + x, position.z + z) + 130;

            float temperature = PerlinNoise::Noise_2d(0.3, 1, position.x + x, position.z + z);

            for(int y = 0; y < CHUNK_Y; y ++)
            {
                float noise_3d = PerlinNoise::Noise_3d(1, 1, position.x + x, position.y + y, position.z + z);

                if (noise_3d > y * 0.005 && y > 10) continue;

                if (y > max_y)
                {
                    continue;
                }
                if (y < 60)
                {
                    blocks[y][x][z] = StoneBlock;
                    continue;
                }
                if (temperature > 0.3f)  //É³Ä®
                {
                    blocks[y][x][z] = SandBlock;
                }
                else if (temperature > 0.0f) //ÂÌÖÞ
                {
                    if (y == max_y)
                    {
                        blocks[y][x][z] = GrassBlock;
                    }
                    else
                    {
                        blocks[y][x][z] = ClayBlock;
                    }
                }
                else  //Ñ©µØ
                {
                    if (y == max_y)
                    {
                        blocks[y][x][z] = SnowBlock;
                    }
                    else
                    {
                        blocks[y][x][z] = ClayBlock;
                    }
                }
            }
        }
    }

    updateMesh();
    if (this->leftChunk != nullptr)
    {
        this->leftChunk->updateMesh();
    }
    if (this->rightChunk != nullptr)
    {
        this->rightChunk->updateMesh();
    }
    if (this->frontChunk != nullptr)
    {
        this->frontChunk->updateMesh();
    }
    if (this->backChunk != nullptr)
    {
        this->backChunk->updateMesh();
    }
}

Chunk::~Chunk()
{
    if (leftChunk != nullptr)
    {
        leftChunk->rightChunk = nullptr;
    }
    if (rightChunk != nullptr)
    {
        rightChunk->leftChunk = nullptr;
    }
    if (frontChunk != nullptr)
    {
        frontChunk->backChunk = nullptr;
    }
    if (backChunk != nullptr)
    {
        backChunk->frontChunk = nullptr;
    }
}

void Chunk::updateMesh()
{
    vOffsets.clear();
    matrices.clear();
    for (int y = 0; y < CHUNK_Y; y++)
    {
        for (int x = 0; x < CHUNK_X; x++)
        {
            for (int z = 0; z < CHUNK_Z; z++)
            {
                const Block& block = blocks[y][x][z];
                if (block.type() == Air)
                {
                    continue;
                }
                bool leftBlockTrans;
                bool rightBlockTrans;
                bool frontBlockTrans;
                bool backBlockTrans;
                bool topBlockTrans;
                bool bottomBlockTrans;
                //Y
                if (y == 0)
                {
                    topBlockTrans = blocks[y + 1][x][z].isTransparent();
                    bottomBlockTrans = true;
                }
                else if (y == CHUNK_Y - 1)
                {
                    bottomBlockTrans = blocks[y - 1][x][z].isTransparent();
                    topBlockTrans = true;
                }
                else 
                {
                    topBlockTrans = blocks[y + 1][x][z].isTransparent();
                    bottomBlockTrans = blocks[y - 1][x][z].isTransparent();
                }
                //X
                if (x == 0)
                {
                    rightBlockTrans = blocks[y][x + 1][z].isTransparent();
                    if (leftChunk != nullptr)
                    {
                        leftBlockTrans = leftChunk->blocks[y][CHUNK_X - 1][z].isTransparent();
                    }
                    else leftBlockTrans = false;
                }
                else if (x == CHUNK_X - 1)
                {
                    leftBlockTrans = blocks[y][x - 1][z].isTransparent();
                    if (rightChunk != nullptr)
                    {
                        rightBlockTrans = rightChunk->blocks[y][0][z].isTransparent();
                    }
                    else rightBlockTrans = false;
                }
                else
                {
                    rightBlockTrans = blocks[y][x + 1][z].isTransparent();
                    leftBlockTrans = blocks[y][x - 1][z].isTransparent();
                }
                //Z
                if (z == 0)
                {
                    frontBlockTrans = blocks[y][x][z + 1].isTransparent();
                    if (backChunk != nullptr)
                    {
                        backBlockTrans = backChunk->blocks[y][x][CHUNK_Z - 1].isTransparent();
                    }
                    else backBlockTrans = false;
                }
                else if (z == CHUNK_Z - 1)
                {
                    backBlockTrans = blocks[y][x][z - 1].isTransparent();
                    if (frontChunk != nullptr)
                    {
                        frontBlockTrans = frontChunk->blocks[y][x][0].isTransparent();
                    }
                    else frontBlockTrans = false;
                }
                else
                {
                    frontBlockTrans = blocks[y][x][z + 1].isTransparent();
                    backBlockTrans = blocks[y][x][z - 1].isTransparent();
                }

                //update
                if (leftBlockTrans)
                {
                    vOffsets.push_back(block.getLeftTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x - 0.5f, y, z)) * Rotate_left);
                }
                if (rightBlockTrans)
                {
                    vOffsets.push_back(block.getRightTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x + 0.5f, y, z)) * Rotate_right);
                }
                if (topBlockTrans)
                {
                    vOffsets.push_back(block.getTopTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y + 0.5f, z)) * Rotate_top);
                }
                if (bottomBlockTrans)
                {
                    vOffsets.push_back(block.getBottomTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y - 0.5f, z)) * Rotate_bottom);
                }
                if (frontBlockTrans)
                {
                    vOffsets.push_back(block.getFrontTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z + 0.5f)) * Rotate_front);
                }
                if (backBlockTrans)
                {
                    vOffsets.push_back(block.getBackTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z - 0.5f)) * Rotate_back);
                }
            }
        }
    }
}

bool Chunk::dig(int y, int x, int z)
{
    if (y < 0 || y >= CHUNK_Y || x < 0 || x >= CHUNK_X || z < 0 || z >= CHUNK_Z) return false;

    if (blocks[y][x][z].type() != Air)
    {
        blocks[y][x][z] = Air;
        updateMesh();
        if (this->leftChunk != nullptr)
        {
            this->leftChunk->updateMesh();
        }
        if (this->rightChunk != nullptr)
        {
            this->rightChunk->updateMesh();
        }
        if (this->frontChunk != nullptr)
        {
            this->frontChunk->updateMesh();
        }
        if (this->backChunk != nullptr)
        {
            this->backChunk->updateMesh();
        }
        return true;
    }
    return false;
}
