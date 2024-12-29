#ifndef __RENDERER_H__
#define __RENDERER_H__
#include "Mesh.h"
#include <glm/glm.hpp>

class Renderer
{
public:
    static Renderer &GetInstance();

    void init();
    void drawBlocks();
    void updateSquares(const std::vector<float> &vOffsets, const std::vector<glm::mat4> &matrices);

    uint32_t getSquareCount() const { return squareCount; }

private:
    Renderer();

    Mesh squareMesh;
    uint32_t squareCount;
};
#endif