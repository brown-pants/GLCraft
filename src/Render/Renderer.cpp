#include "Renderer.h"
#include "../Application/Application.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/TextureManager.h"
#include "../Player/Player.h"
#include "../World/World.h"

Renderer &Renderer::GetInstance()
{
    static Renderer __instance;
    return __instance;
}

Renderer::Renderer() : squareCount(0), waterCount(0)
{
}

void Renderer::init()
{
    /*          square mesh         */
    float square_vertices[] = {
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f, // right top
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // right bottom
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left bottom
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f  // left top
    };

    unsigned int square_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //create
    squareMesh.vao.create();
    squareMesh.vbo.create();
    squareMesh.ebo.create();

    //vertex vbo
    squareMesh.vbo.setData(sizeof(square_vertices), square_vertices);
    squareMesh.ebo.setData(sizeof(square_indices), square_indices);
    squareMesh.vao.addVBO(0, squareMesh.vbo, 3, 5 * sizeof(float), 0);
    squareMesh.vao.addVBO(1, squareMesh.vbo, 2, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    squareMesh.vao.setEBO(squareMesh.ebo);

    //instance vbo
    squareMesh.instance_vbos.push_back(GLVertexBuffer());   //instance_vOffset
    squareMesh.instance_vbos.push_back(GLVertexBuffer());   //instance_matrix
    squareMesh.instance_vbos[0].create();
    squareMesh.instance_vbos[1].create();
    
    //bind instance_vOffset
    squareMesh.vao.addVBO(2, squareMesh.instance_vbos[0], 1, sizeof(float), 0);

    //bind instance_matrix
    squareMesh.vao.addVBO(3, squareMesh.instance_vbos[1], 4, sizeof(glm::mat4), 0);
    squareMesh.vao.addVBO(4, squareMesh.instance_vbos[1], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4)));
    squareMesh.vao.addVBO(5, squareMesh.instance_vbos[1], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4) * 2));
    squareMesh.vao.addVBO(6, squareMesh.instance_vbos[1], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4) * 3));
    
    //setup attrib divisor
    squareMesh.vao.bind();
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    squareMesh.vao.unbind();

    /*          water mesh         */
    //create
    waterMesh.vao.create();
    waterMesh.vbo.create();
    waterMesh.ebo.create();

    //vertex vbo
    waterMesh.vbo.setData(sizeof(square_vertices), square_vertices);
    waterMesh.ebo.setData(sizeof(square_indices), square_indices);
    waterMesh.vao.addVBO(0, waterMesh.vbo, 3, 5 * sizeof(float), 0);
    waterMesh.vao.addVBO(1, waterMesh.vbo, 2, 5 * sizeof(float), (const void *)(3 * sizeof(float)));
    waterMesh.vao.setEBO(waterMesh.ebo);

    //instance vbo
    waterMesh.instance_vbos.push_back(GLVertexBuffer());   //instance_matrix
    waterMesh.instance_vbos[0].create();
    
    //bind instance_matrix
    waterMesh.vao.addVBO(2, waterMesh.instance_vbos[0], 4, sizeof(glm::mat4), 0);
    waterMesh.vao.addVBO(3, waterMesh.instance_vbos[0], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4)));
    waterMesh.vao.addVBO(4, waterMesh.instance_vbos[0], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4) * 2));
    waterMesh.vao.addVBO(5, waterMesh.instance_vbos[0], 4, sizeof(glm::mat4), (const void *)(sizeof(glm::vec4) * 3));
    
    //setup attrib divisor
    waterMesh.vao.bind();
    glVertexAttribDivisor(2, 1);
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    waterMesh.vao.unbind();

    /*          crosshair mesh         */
    float c = 0.002f;
    float crosshair_vertices[] = {
        -c, 0.0f,
        c, 0.0f,
        0.0f, c,
        0.0f, -c
    };
    crosshairMesh.vao.create();
    crosshairMesh.vbo.create();
    crosshairMesh.vbo.setData(sizeof(crosshair_vertices), crosshair_vertices);
    crosshairMesh.vao.addVBO(0, crosshairMesh.vbo, 2, 2 * sizeof(float), 0);

    /*          planet mesh         */
    c = 50.0f;
    float z = -100.0f;
    float planet_vertices[] = {
         c, c, z,   1.0f, 1.0f,  // right top
        c, -c, z,   1.0f, 0.0f,  // right bottom
        -c, -c, z,  0.0f, 0.0f,  // left bottom
        -c, c, z,   0.0f, 1.0f   // left top
    };

    unsigned int planet_indices[] = {
        3, 1, 0, // first triangle
        3, 2, 1  // second triangle
    };

    planetMesh.vao.create();
    planetMesh.vbo.create();
    planetMesh.ebo.create();

    planetMesh.vbo.setData(sizeof(planet_vertices), planet_vertices);
    planetMesh.vao.addVBO(0, planetMesh.vbo, 3, 5 * sizeof(float), 0);
    planetMesh.vao.addVBO(1, planetMesh.vbo, 2, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
    planetMesh.ebo.setData(sizeof(planet_indices), planet_indices);
    planetMesh.vao.setEBO(planetMesh.ebo);

    /*          loading mesh         */
    c = 0.7;
    float loading_vertices[] = {
        c, c - 0.5f,    1.0f, 1.0f,  // right top
        c, -c + 0.4f,   1.0f, 0.0f,  // right bottom
        -c, -c + 0.4f,  0.0f, 0.0f,  // left bottom
        -c, c - 0.5f,   0.0f, 1.0f   // left top
    };

    unsigned int loading_indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    
    loadingMesh.vao.create();
    loadingMesh.vbo.create();
    loadingMesh.ebo.create();
    
    loadingMesh.vbo.setData(sizeof(loading_vertices), loading_vertices);
    loadingMesh.vao.addVBO(0, loadingMesh.vbo, 2, 4 * sizeof(float), 0);
    loadingMesh.vao.addVBO(1, loadingMesh.vbo, 2, 4 * sizeof(float), (const void *)(2 * sizeof(float)));
    loadingMesh.ebo.setData(sizeof(loading_indices), loading_indices);
    loadingMesh.vao.setEBO(loadingMesh.ebo);

    /*          loading depth map frame buffer         */
    depthMap.create();
    depthMapFBO.create();

    depthMap.setData(shadow_width, shadow_height);
    depthMapFBO.setDepthMap(depthMap);
}

void Renderer::drawBlocks()
{
    GLShader &squareShader = ShaderManager::GetInstance().getShader(ShaderManager::Square);
    GLTexture2D &squareTexture = TextureManager::GetInstance().getTexture2d();
    Camera &camera = Player::GetInstance().getCamera();

    //setup shader uniform variable
    squareShader.setMat4("view", camera.getViewMatrix());
    squareShader.setMat4("projection", camera.getProjectionMatrix());
    squareShader.setInt("tex2d_count", TextureManager::GetInstance().tex2d_count);
    squareShader.setVec3("sunPos", World::RunningWorld->getSunPosition());
    squareShader.setVec3("moonPos", World::RunningWorld->getMoonPosition());
    squareShader.setVec3("skyColor", World::RunningWorld->getSkyColor());
    squareShader.setMat4("lightSpaceMatrix", getLightSpaceMatrix());
    squareShader.setInt("tex", 0);
    squareShader.setInt("shadowMap", 1);

    // dive
    if (Player::GetInstance().isDive())
    {
        squareShader.setVec3("diveColor", glm::vec3(0.3, 0.5, 0.7));
        squareShader.setFloat("density", 0.01);
        squareShader.setFloat("gradient", 8.5);
    }
    else
    {
        squareShader.setVec3("diveColor", glm::vec3(1.0f));
        squareShader.setFloat("density", 0.005);
        squareShader.setFloat("gradient", 9.5);
    }
    
    // bind textures
    glActiveTexture(GL_TEXTURE0);
    squareTexture.bind();
    glActiveTexture(GL_TEXTURE1);
    depthMap.bind();

    //render...
    squareShader.bind();
    squareMesh.vao.bind();

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, squareCount);

    squareMesh.vao.unbind();
    squareShader.unbind();
    
    squareTexture.unbind();
    depthMap.unbind();
}

void Renderer::drawWater()
{
    glDisable(GL_CULL_FACE);
    static float waterMapOffset = 0.0f;
    GLShader &waterShader = ShaderManager::GetInstance().getShader(ShaderManager::Water);
    GLTexture2D &waterMap = TextureManager::GetInstance().getWaterMap();
    Camera &camera = Player::GetInstance().getCamera();

    //setup shader uniform variable
    waterShader.setMat4("view", camera.getViewMatrix());
    waterShader.setMat4("projection", camera.getProjectionMatrix());
    waterShader.setVec3("sunPos", World::RunningWorld->getSunPosition());
    waterShader.setVec3("moonPos", World::RunningWorld->getMoonPosition());
    waterShader.setVec3("skyColor", World::RunningWorld->getSkyColor());
    waterShader.setFloat("texOffset", waterMapOffset );
    waterShader.setInt("tex", 0);

    // bind textures
    glActiveTexture(GL_TEXTURE0);
    waterMap.bind();

    waterMapOffset += 0.001f * 100.0f / Application::GetFps();
    if (waterMapOffset >= 16)
    {
        waterMapOffset = 0;
    }

    //render...
    waterShader.bind();
    waterMesh.vao.bind();

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, waterCount);

    waterMesh.vao.unbind();
    waterShader.unbind();
    waterMap.unbind();
    glEnable(GL_CULL_FACE);
}

void Renderer::drawCrosshair()
{
    glDisable(GL_DEPTH_TEST);
    GLShader& squareShader = ShaderManager::GetInstance().getShader(ShaderManager::Crosshair);
    Camera& camera = Player::GetInstance().getCamera();
    squareShader.setMat4("projection", camera.getProjectionMatrix());
    squareShader.bind();
    crosshairMesh.vao.bind();
    glDrawArrays(GL_LINES, 0, 4);
    crosshairMesh.vao.unbind();
    squareShader.unbind();
    glEnable(GL_DEPTH_TEST);
}

void Renderer::drawPlanet()
{
    glDepthFunc(GL_LEQUAL);
    GLShader& shader = ShaderManager::GetInstance().getShader(ShaderManager::Planet);
    Camera& camera = Player::GetInstance().getCamera();
    
    //setup shader uniform variable
    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
    shader.setMat4("view", view);
    shader.setMat4("projection", camera.getProjectionMatrix());
    shader.setInt("tex", 0);

    planetMesh.vao.bind();
    shader.setMat4("model", World::RunningWorld->getSunModelMatrix());
    shader.bind();

    //draw sun
    glActiveTexture(GL_TEXTURE0);
    TextureManager::GetInstance().getSunTexture().bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    //draw moon
    shader.setMat4("model", World::RunningWorld->getMoonModelMatrix());
    shader.bind();
    glActiveTexture(GL_TEXTURE0);
    TextureManager::GetInstance().getMoonTexture().bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    planetMesh.vao.unbind();
    shader.unbind();
    glDepthFunc(GL_LESS);
}

void Renderer::drawLoadingBackground()
{
    GLShader &shader = ShaderManager::GetInstance().getShader(ShaderManager::Texture);
    GLTexture2D &texture = TextureManager::GetInstance().getLoadingTexture();
    loadingMesh.vao.bind();
    texture.bind();
    shader.bind();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    shader.unbind();
    texture.unbind();
    loadingMesh.vao.unbind();
}

glm::mat4 Renderer::getLightSpaceMatrix()
{
    glm::vec3 playerPos = Player::GetInstance().getInfo().position;
    glm::vec3 lightDir = World::RunningWorld->sunAngle() < 180.0f 
        ? World::RunningWorld->getSunPosition() 
        : World::RunningWorld->getMoonPosition();
    lightDir = -glm::normalize(lightDir);
    
    const float REGION_SIZE = 32.0f;
    glm::vec3 regionCenter = glm::vec3(
        round(playerPos.x / REGION_SIZE) * REGION_SIZE,
        round(playerPos.y / REGION_SIZE) * REGION_SIZE,
        round(playerPos.z / REGION_SIZE) * REGION_SIZE
    );
    
    glm::vec3 lightPos = regionCenter - lightDir * 300.0f;
    glm::vec3 lookAtPos = regionCenter;
    
    float near_plane = 1.0f, far_plane = 800.0f;
    glm::mat4 lightProjection = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(lightPos, lookAtPos, glm::vec3(0.0f, 1.0f, 0.0f));
    
    return lightProjection * lightView;
}

void Renderer::drawDepthMap()
{
    glViewport(0, 0, shadow_width, shadow_height);
    depthMapFBO.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
    GLShader &shader = ShaderManager::GetInstance().getShader(ShaderManager::Depth);
    shader.setMat4("lightSpaceMatrix", getLightSpaceMatrix());
    shader.bind();
    squareMesh.vao.bind();
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, squareCount);
    squareMesh.vao.unbind();
    shader.unbind();
    depthMapFBO.unbind();

    int window_width = Application::GetApp()->getWindow()->width();
    int window_height = Application::GetApp()->getWindow()->height();
    glViewport(0, 0, window_width, window_height);
}

void Renderer::updateSquares(const std::vector<float> &vOffsets, const std::vector<glm::mat4> &matrices)
{
    if (vOffsets.size() == 0 || matrices.size() == 0) return;
    squareMesh.instance_vbos[0].setData(vOffsets.size() * sizeof(float), &vOffsets[0]);
    squareMesh.instance_vbos[1].setData(matrices.size() * sizeof(glm::mat4), &matrices[0][0]);
    squareCount = matrices.size();
}

void Renderer::updateWater(const std::vector<glm::mat4> &matrices)
{
    if (matrices.size() == 0) return;
    waterMesh.instance_vbos[0].setData(matrices.size() * sizeof(glm::mat4), &matrices[0][0]);
    waterCount = matrices.size();
}

void Renderer::clearMeshes()
{
    loadingMesh.clear();
    squareMesh.clear();
    planetMesh.clear();
    crosshairMesh.clear();
    waterMesh.clear();
    depthMap.deleteObj();
    depthMapFBO.deleteObj();
}