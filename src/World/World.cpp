#include "World.h"
#include "../Render/Renderer.h"
#include "../Player/Player.h"
#include "../Application/Application.h"

#include <iostream>
#include <mutex>

#define CHUNK_LOAD_DISTANCE 250
#define CHUNK_DELETE_DISTANCE 300

World* World::RunningWorld = nullptr;

std::mutex mtx;

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
}

void World::updateRenderMeshes()
{
    if (mtx.try_lock())
    {
        Renderer::GetInstance().updateSquares(vOffsets, matrices);
        mtx.unlock();
    }
}

bool World::digTest(const glm::vec3& pos)
{
    int chunkX = floor(pos.x / CHUNK_X) * CHUNK_X;
    int chunkZ = floor(pos.z / CHUNK_Z) * CHUNK_Z;
    Chunk* chunk = getChunk(glm::vec3(chunkX, 0.0f, chunkZ));
    if (chunk != nullptr)
    {
        int blockX = (int)floor(pos.x) % CHUNK_X;
        int blockZ = (int)floor(pos.z) % CHUNK_Z;
        int blockY = (int)pos.y;
        if (blockX < 0)
        {
            blockX += CHUNK_X;
        }
        if (blockZ < 0)
        {
            blockZ += CHUNK_Z;
        }
        std::lock_guard<std::mutex> guard(mtx);
        if (chunk->dig(blockY, blockX, blockZ))
        {
            updateMeshes();
            return true;
        }
    }
    return false;
}

bool World::putTest(const glm::vec3& pos, const glm::vec3& dir, Block_Type block_type)
{
    if (touchTest(pos))
    {
        glm::vec3 putPos = pos - dir;
        int chunkX = floor(putPos.x / CHUNK_X) * CHUNK_X;
        int chunkZ = floor(putPos.z / CHUNK_Z) * CHUNK_Z;
        Chunk* chunk = getChunk(glm::vec3(chunkX, 0.0f, chunkZ));
        if (chunk != nullptr)
        {
            int blockX = (int)floor(putPos.x) % CHUNK_X;
            int blockZ = (int)floor(putPos.z) % CHUNK_Z;
            int blockY = (int)putPos.y;
            if (blockX < 0)
            {
                blockX += CHUNK_X;
            }
            if (blockZ < 0)
            {
                blockZ += CHUNK_Z;
            }
            std::lock_guard<std::mutex> guard(mtx);
            if (chunk->put(blockY, blockX, blockZ, block_type))
            {
                updateMeshes();
                return true;
            }
        }
    }
    return false;
}

bool World::touchTest(const glm::vec3& pos)
{
    int chunkX = floor(pos.x / CHUNK_X) * CHUNK_X;
    int chunkZ = floor(pos.z / CHUNK_Z) * CHUNK_Z;
    Chunk* chunk = getChunk(glm::vec3(chunkX, 0.0f, chunkZ));
    if (chunk != nullptr)
    {
        int blockX = (int)floor(pos.x) % CHUNK_X;
        int blockZ = (int)floor(pos.z) % CHUNK_Z;
        int blockY = (int)pos.y;
        if (blockX < 0)
        {
            blockX += CHUNK_X;
        }
        if (blockZ < 0)
        {
            blockZ += CHUNK_Z;
        }
        int type = chunk->getBlockType(blockY, blockX, blockZ);
        if (type != -1 && type != Air)
        {
            return true;
            
        }
    }
    return false;
}

void World::updatePlanet()
{
    float a = 1.5f / Application::GetFps();
    sunRotateAngle += a;
    if (sunRotateAngle >= 360.0f)
    {
        sunRotateAngle = 0.0f;
    }
}

glm::vec3 World::getSkyColor() const
{
    float h = getSunPosition().y;
    if (h < 0) return glm::vec3(0.0f);
    else return glm::vec3(0.5f / 100.0f * h, 0.8f / 100.0f * h, 1.0f / 100.0f * h);
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
}

World::World() : m_running(true)
{
    RunningWorld = this;
}

void World::init(const glm::vec3& playerPos)
{
    float x = floor(playerPos.x / CHUNK_X) * CHUNK_X;
    float z = floor(playerPos.z / CHUNK_Z) * CHUNK_Z;
    loadChunk(glm::vec3(x, 0, z));
    update();
    th_loadWorld = new std::thread([this]() {
        while (m_running)
        {
            this->update();
        }
        });
}

void World::stop()
{
    m_running = false;
}

World::~World()
{
    th_loadWorld->join();
    delete th_loadWorld;
}

void World::update()
{
    bool isUpdateMeshes = false;
    glm::vec3 playerPos = Player::GetInstance().getPosition();
    for(auto it = edgeChunks.begin(); it != edgeChunks.end(); )
    {
        if (!m_running)
        {
            break;
        }
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
            isUpdateMeshes = true;
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

    if (isUpdateMeshes)
    {
        std::lock_guard<std::mutex> guard(mtx);
        updateMeshes();
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