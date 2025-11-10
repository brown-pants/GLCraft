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
    void drawCrosshair();
    void drawPlanet();
    void drawWater();
    void drawLoadingBackground();
    void drawDepthMap();
    void updateSquares(const std::vector<float> &vOffsets, const std::vector<glm::mat4> &matrices);
    void updateWater(const std::vector<glm::mat4> &matrices);
    void clearMeshes();

    uint32_t getSquareCount() const { return squareCount + waterCount; }

private:
    Renderer();
    glm::mat4 getLightSpaceMatrix();

    Mesh loadingMesh;
    Mesh squareMesh;
    Mesh planetMesh;
    Mesh crosshairMesh;
    Mesh waterMesh;

    GLDepthMap depthMap;
    GLFrameBuffer depthMapFBO;
    
    const int shadow_width = 1024;
    const int shadow_height = 1024;
    
    uint32_t squareCount;
    uint32_t waterCount;
};
#endif