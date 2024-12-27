#include "Chunk.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

glm::mat4 Rotate90_v = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
glm::mat4 Rotate90_h = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));


Chunk::Chunk(glm::vec3 position) : position(position)
{
    for(int y = 0; y < CHUNK_Y; y ++)
    {
        for(int x = 0; x < CHUNK_X; x ++)
        {
            for(int z = 0; z < CHUNK_Z; z ++)
            {
                float _y = (sin(3.1415 * x / (CHUNK_X - 1)) + sin(3.1415 * z / (CHUNK_Z - 1))) / 2 * 5;
                if((int)_y == y)
                {
                    blocks[y][x][z] = GrassBlock;
                    vOffset.push_back(blocks[y][x][z].getLeftTexture());
                    vOffset.push_back(blocks[y][x][z].getRightTexture());
                    vOffset.push_back(blocks[y][x][z].getTopTexture());
                    vOffset.push_back(blocks[y][x][z].getBottomTexture());
                    vOffset.push_back(blocks[y][x][z].getFrontTexture());
                    vOffset.push_back(blocks[y][x][z].getBackTexture());
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x - 0.5f, y, z)) * Rotate90_v);
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x + 0.5f, y, z)) * Rotate90_v);
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y + 0.5f, z)) * Rotate90_h);
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y - 0.5f, z)) * Rotate90_h);
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z + 0.5f)));
                    matrices.push_back(glm::translate(glm::mat4(1.0f), position + glm::vec3(x, y, z - 0.5f)));
                }
                
            }
        }
    }
}