#include "World.h"
#include "../Render/Renderer.h"
#include "../Player/Player.h"

#include <iostream>

void World::loadChunk(const glm::vec3 &position)
{
    Chunk *chunk = new Chunk(position);
    chunks.push_back(chunk);
    edgeChunks.push_back(chunk);
    updateMeshes();
}

void World::updateMeshes()
{
    std::vector<float> vOffsets; 
    std::vector<glm::mat4> matrices; 
    for(auto chunk : chunks)
    {
        vOffsets.insert(vOffsets.end(), chunk->vOffset.begin(), chunk->vOffset.end());
        matrices.insert(matrices.end(), chunk->matrices.begin(), chunk->matrices.end());
    }
    Renderer::GetInstance().updateSquares(vOffsets, matrices);
}

World::World()
{
    loadChunk(glm::vec3(0, 0, 0));
}

void World::update()
{
    glm::vec3 playerPos = Player::GetInstance().getCamera().position;
    for(auto it = edgeChunks.begin(); it != edgeChunks.end(); )
    {
        Chunk *edgeChunk = *it;
        if(glm::length(playerPos - edgeChunk->position) < 100)
        {
            glm::vec3 leftChunkPos = edgeChunk->position - glm::vec3(CHUNK_X, 0, 0);
            glm::vec3 rightChunkPos = edgeChunk->position + glm::vec3(CHUNK_X, 0, 0);
            glm::vec3 frontChunkPos = edgeChunk->position + glm::vec3(0, 0, CHUNK_Z);
            glm::vec3 backChunkPos = edgeChunk->position - glm::vec3(0, 0, CHUNK_Z);
            if(getChunk(leftChunkPos) == nullptr)
            {
                loadChunk(leftChunkPos);
            }
            if(getChunk(rightChunkPos) == nullptr)
            {
                loadChunk(rightChunkPos);
            }
            if(getChunk(frontChunkPos) == nullptr)
            {
                loadChunk(frontChunkPos);
            }
            if(getChunk(backChunkPos) == nullptr)
            {
                loadChunk(backChunkPos);
            }
            it = edgeChunks.erase(it);
        }
        else if(glm::length(playerPos - edgeChunk->position) > 200)
        {
            chunks.erase(std::find(chunks.begin(), chunks.end(), edgeChunk));
            it = edgeChunks.erase(it);
        }
        else
        {
            it ++;
        }
    }
}

Chunk *World::getChunk(const glm::vec3 position)
{
    for(auto chunk : chunks)
    {
        if(chunk->position == position)
        {
            return chunk;
        }
    }
    return nullptr;
}