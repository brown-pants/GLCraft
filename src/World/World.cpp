#include "World.h"
#include "../Render/Renderer.h"
#include "../Player/Player.h"

#include <iostream>

#define CHUNK_LOAD_DISTANCE 250
#define CHUNK_DELETE_DISTANCE 300

void World::loadChunk(const glm::vec3 &position)
{
    glm::vec3 leftChunkPos = position - glm::vec3(CHUNK_X, 0, 0);
    glm::vec3 rightChunkPos = position + glm::vec3(CHUNK_X, 0, 0);
    glm::vec3 frontChunkPos = position + glm::vec3(0, 0, CHUNK_Z);
    glm::vec3 backChunkPos = position - glm::vec3(0, 0, CHUNK_Z);

    Chunk* leftChunk = getChunk(leftChunkPos);
    Chunk* rightChunk = getChunk(rightChunkPos);
    Chunk* frontChunk = getChunk(frontChunkPos);
    Chunk* backChunk = getChunk(backChunkPos);

    Chunk *chunk = new Chunk(position, leftChunk, rightChunk, frontChunk, backChunk);
    chunks.push_back(chunk);
    edgeChunks.push_back(chunk);
    updateMeshes();
}

void World::updateMeshes()
{ 
    vOffsets.clear();
    matrices.clear();
    for(auto chunk : chunks)
    {
        std::vector<float> &chunk_vOffsets = chunk->getVOffsets();
        std::vector<glm::mat4>& chunk_matrices = chunk->getMatrices();
        vOffsets.insert(vOffsets.end(), chunk_vOffsets.begin(), chunk_vOffsets.end());
        matrices.insert(matrices.end(), chunk_matrices.begin(), chunk_matrices.end());
    }
    Renderer::GetInstance().updateSquares(vOffsets, matrices);
}

World::World(const glm::vec3& playerPos)
{
    float x = (int)playerPos.x / CHUNK_X;
    float z = (int)playerPos.z / CHUNK_X;
    loadChunk(glm::vec3(x, 0, z));
}

void World::update()
{
    glm::vec3 playerPos = Player::GetInstance().getCamera().position;
    for(auto it = edgeChunks.begin(); it != edgeChunks.end(); )
    {
        Chunk *edgeChunk = *it;
        glm::vec3 chunkPos = edgeChunk->getPosition();

        glm::vec3 leftChunkPos = chunkPos - glm::vec3(CHUNK_X, 0, 0);
        glm::vec3 rightChunkPos = chunkPos + glm::vec3(CHUNK_X, 0, 0);
        glm::vec3 frontChunkPos = chunkPos + glm::vec3(0, 0, CHUNK_Z);
        glm::vec3 backChunkPos = chunkPos - glm::vec3(0, 0, CHUNK_Z);

        Chunk *leftChunk = edgeChunk->getLeftChunk();
        Chunk *rightChunk = edgeChunk->getRightChunk();
        Chunk *frontChunk = edgeChunk->getFrontChunk();
        Chunk *backChunk = edgeChunk->getBackChunk();

        float distance = glm::length(glm::vec2(playerPos.x - chunkPos.x, playerPos.z - chunkPos.z));

        if(distance < CHUNK_LOAD_DISTANCE)
        {
            if(leftChunk == nullptr)
            {
                loadChunk(leftChunkPos);
            }
            if(rightChunk == nullptr)
            {
                loadChunk(rightChunkPos);
            }
            if(frontChunk == nullptr)
            {
                loadChunk(frontChunkPos);
            }
            if(backChunk == nullptr)
            {
                loadChunk(backChunkPos);
            }
            it = edgeChunks.erase(it);
        }
        else if(distance > CHUNK_DELETE_DISTANCE)
        {
            if(leftChunk != nullptr && std::find(edgeChunks.begin(), edgeChunks.end(), leftChunk) == edgeChunks.end())
            {
                edgeChunks.push_back(leftChunk);
            }
            if(rightChunk != nullptr && std::find(edgeChunks.begin(), edgeChunks.end(), rightChunk) == edgeChunks.end())
            {
                edgeChunks.push_back(rightChunk);
            }
            if(frontChunk != nullptr && std::find(edgeChunks.begin(), edgeChunks.end(), frontChunk) == edgeChunks.end())
            {
                edgeChunks.push_back(frontChunk);
            }
            if(backChunk != nullptr && std::find(edgeChunks.begin(), edgeChunks.end(), backChunk) == edgeChunks.end())
            {
                edgeChunks.push_back(backChunk);
            }
            auto iter = std::find(chunks.begin(), chunks.end(), edgeChunk);
    
            chunks.erase(iter);
            it = edgeChunks.erase(it);
            delete edgeChunk;
        }
        else
        {
            it ++;
        }
    }
}

Chunk *World::getChunk(const glm::vec3 &position)
{
    for(auto chunk : chunks)
    {
        if(chunk->getPosition() == position)
        {
            return chunk;
        }
    }
    return nullptr;
}