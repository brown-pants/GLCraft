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
    vOffsets.clear();
    matrices.clear();
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
    /*m_thread = new std::thread([&]() 
        {
        while (true)
        {
            update();
        }
        }
    );*/
}

void World::update()
{
    glm::vec3 playerPos = Player::GetInstance().getCamera().position;
    for(auto it = edgeChunks.begin(); it != edgeChunks.end(); )
    {
        Chunk *edgeChunk = *it;

        glm::vec3 leftChunkPos = edgeChunk->position - glm::vec3(CHUNK_X, 0, 0);
        glm::vec3 rightChunkPos = edgeChunk->position + glm::vec3(CHUNK_X, 0, 0);
        glm::vec3 frontChunkPos = edgeChunk->position + glm::vec3(0, 0, CHUNK_Z);
        glm::vec3 backChunkPos = edgeChunk->position - glm::vec3(0, 0, CHUNK_Z);

        Chunk *leftChunk = getChunk(leftChunkPos);
        Chunk *rightChunk = getChunk(rightChunkPos);
        Chunk *frontChunk = getChunk(frontChunkPos);
        Chunk *backChunk = getChunk(backChunkPos);

        float length = glm::length(glm::vec2(playerPos.x - edgeChunk->position.x, playerPos.z - edgeChunk->position.z));

        if(length < 100)
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
        else if(length > 116)
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
        if(chunk->position == position)
        {
            return chunk;
        }
    }
    return nullptr;
}