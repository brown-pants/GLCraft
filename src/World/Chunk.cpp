#include "Chunk.h"
#include "../Application/Application.h"
#include "../Math/Noise/PerlinNoise.h"
#include "../Player/Player.h"
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 Rotate_left = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_right = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_top = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 Rotate_bottom = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
glm::mat4 Rotate_front = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate_back = glm::mat4(1.0f);

glm::mat4 Translate_cactus = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0f, 0.1f));
glm::mat4 Translate_global = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.5f));

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

    float tree_noise = PerlinNoise::Noise_2d(1, 1, position.x, position.z);
    int tree_x = abs((int)(tree_noise * 100)) % (CHUNK_X - 6) + 3;
    int tree_z = abs((int)(tree_noise * 100)) % (CHUNK_Z - 6) + 3;

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
                /*float noise_3d = PerlinNoise::Noise_3d(1, 1, position.x + x, position.y + y, position.z + z);
                if (noise_3d > y * 0.005 && y > 10) continue;*/

                if (y > max_y)
                {
                    int type = Application::GetApp()->blockChanged(position.x, position.z, x, y, z);
                    if (type != -1)
                    {
                        blocks[y][x][z] = (Block_Type)type;
                    }
                }
                else if (y < 60)
                {
                    loadBlock(x, y, z, StoneBlock);
                }
                else if (temperature > 0.3f)  //ɳĮ
                {
                    loadBlock(x, y, z, SandBlock);
                    //������
                    if (y == max_y)
                    {
                        float noise = PerlinNoise::Noise_2d(90, 1, position.x + x, position.z + z);
                        if (noise > 0.8f)
                        {
                            int height = abs((int)(noise * 100)) % 3 + 3;
                            generateCactus(y, x, z, height);
                        }
                    }
                }
                else if (temperature > 0.0f) //����
                {
                    if (y == max_y)
                    {
                        loadBlock(x, y, z, GrassBlock);
                        //generate tree
                        if (tree_noise < 0.3 && x == tree_x && z == tree_z)
                        {
                            int height = abs((int)(tree_noise * 100)) % 5 + 4;
                            generateTree(y, x, z, height);
                        }
                    }
                    else
                    {
                        loadBlock(x, y, z, ClayBlock);
                    }
                }
                else  //ѩ��
                {
                    if (y == max_y)
                    {
                        loadBlock(x, y, z, SnowBlock);
                    }
                    else
                    {
                        loadBlock(x, y, z, ClayBlock);
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
                    if (block.type() == CactusBlock)
                    {
                        if (blocks[y + 1][x][z].type() == CactusBlock)
                        {
                            topBlockTrans = false;
                        }
                        if (blocks[y - 1][x][z].type() == CactusBlock)
                        {
                            bottomBlockTrans = false;
                        }
                    }
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
                if (leftBlockTrans || block.type() == CactusBlock)
                {
                    vOffsets.push_back(block.getLeftTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x - 0.5f, y, z)) * Translate_global * Rotate_left * (block.type() == CactusBlock ? Translate_cactus : glm::mat4(1.0f)));
                }
                if (rightBlockTrans || block.type() == CactusBlock)
                {
                    vOffsets.push_back(block.getRightTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x + 0.5f, y, z)) * Translate_global * Rotate_right * (block.type() == CactusBlock ? Translate_cactus : glm::mat4(1.0f)));
                }
                if (topBlockTrans)
                {
                    vOffsets.push_back(block.getTopTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y + 0.5f, z)) * Translate_global * Rotate_top);
                }
                if (bottomBlockTrans)
                {
                    vOffsets.push_back(block.getBottomTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y - 0.5f, z)) * Translate_global * Rotate_bottom);
                }
                if (frontBlockTrans || block.type() == CactusBlock)
                {
                    vOffsets.push_back(block.getFrontTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z + 0.5f)) * Translate_global * Rotate_front * (block.type() == CactusBlock ? Translate_cactus : glm::mat4(1.0f)));
                }
                if (backBlockTrans || block.type() == CactusBlock)
                {
                    vOffsets.push_back(block.getBackTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z - 0.5f)) * Translate_global * Rotate_back * (block.type() == CactusBlock ? Translate_cactus : glm::mat4(1.0f)));
                }
            }
        }
    }
}

int Chunk::getBlockType(int y, int x, int z)
{
    if (y < 0 || y >= CHUNK_Y || x < 0 || x >= CHUNK_X || z < 0 || z >= CHUNK_Z) return -1;
    return blocks[y][x][z].type();
}

bool Chunk::dig(int y, int x, int z)
{
    if (y < 0 || y >= CHUNK_Y || x < 0 || x >= CHUNK_X || z < 0 || z >= CHUNK_Z) return false;

    if (blocks[y][x][z].type() != Air)
    {
        blocks[y][x][z] = Air;
        Application::GetApp()->changeBlock(position.x, position.z, x, y, z, Air);
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

bool Chunk::put(int y, int x, int z, Block_Type type)
{
    if (y < 0 || y >= CHUNK_Y || x < 0 || x >= CHUNK_X || z < 0 || z >= CHUNK_Z) return false;
    
    if (Player::GetInstance().isBindPhysical())
    {
        Block_Type temp = blocks[y][x][z].type();
        blocks[y][x][z] = type;
        bool sign = Player::GetInstance().obstacleTest(Player::GetInstance().getPosition());
        blocks[y][x][z] = temp;
        if (sign)
        {
            return false;
        }
    }

    if (blocks[y][x][z].type() == Air)
    {
        blocks[y][x][z] = type;
        Application::GetApp()->changeBlock(position.x, position.z, x, y, z, type);
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

void Chunk::generateTree(int y, int x, int z, int height)
{
    for (int i = 1; i <= height; i++)
    {
        loadBlock(x, y + i, z, LogBlock);
    }
    for (int _y = 0; _y < 2; _y++)
    {
        for (int _x = -2; _x <= 2; _x++)
        {
            for (int _z = -2; _z <= 2; _z++)
            {
                if(_x == 0 && _z == 0)
                {
                    continue;
                }
                loadBlock(x + _x, y + _y + height - 1, z + _z, LeaveBlock);
            }
        }
    }

    for (int _x = -1; _x <= 1; _x++)
    {
        for (int _z = -1; _z <= 1; _z++)
        {
            loadBlock(x + _x, height + y + 1, z + _z, LeaveBlock);
        }
    }
    loadBlock(x, height + y + 2, z, LeaveBlock);
    loadBlock(x - 1, height + y + 2, z, LeaveBlock);
    loadBlock(x + 1, height + y + 2, z, LeaveBlock);
    loadBlock(x, height + y + 2, z - 1, LeaveBlock);
    loadBlock(x, height + y + 2, z + 1, LeaveBlock);
}

void Chunk::generateCactus(int y, int x, int z, int height)
{
    for (int i = 1; i <= height; i++)
    {
        loadBlock(x, y + i, z, CactusBlock);
    }
}

void Chunk::loadBlock(int x, int y, int z, Block_Type block_type)
{
    int type = Application::GetApp()->blockChanged(position.x, position.z, x, y, z);
    if (type != -1)
    {
        blocks[y][x][z] = (Block_Type)type;
    }
    else
    {
        blocks[y][x][z] = block_type;
    }
}