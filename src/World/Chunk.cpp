#include "Chunk.h"
#include "../Application/Application.h"
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 Rotate90_v = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate90_h = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

float freq = 0.5; // ÆµÂÊ
float amplitude = 0.5; // Õñ·ù

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
            //float _y = (sin(3.1415 * x / (CHUNK_X - 1)) + sin(3.1415 * z / (CHUNK_Z - 1))) / 2 * 5;
            float noise_y = Application::GetNoise().GetNoise((x + position.x) * freq, (z + position.z) * freq) * amplitude;
            float __y = (int)(noise_y * 100) + 100;
            for(int y = 0; y < CHUNK_Y; y ++)
            {
                //if((int)_y >= y)
                if (y > __y) continue;
                if(y == __y)
                {
                    blocks[y][x][z] = GrassBlock;
                }
                else if(y > 50)
                {
                    blocks[y][x][z] = ClayBlock;
                }
                else
                {
                    blocks[y][x][z] = StoneBlock;
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
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x - 0.5f, y, z)) * Rotate90_v);
                }
                if (rightBlockTrans)
                {
                    vOffsets.push_back(block.getRightTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x + 0.5f, y, z)) * Rotate90_v);
                }
                if (topBlockTrans)
                {
                    vOffsets.push_back(block.getTopTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y + 0.5f, z)) * Rotate90_h);
                }
                if (bottomBlockTrans)
                {
                    vOffsets.push_back(block.getBottomTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y - 0.5f, z)) * Rotate90_h);
                }
                if (frontBlockTrans)
                {
                    vOffsets.push_back(block.getFrontTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z + 0.5f)));
                }
                if (backBlockTrans)
                {
                    vOffsets.push_back(block.getBackTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z - 0.5f)));
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
