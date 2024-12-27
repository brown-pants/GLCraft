#include "Renderer.h"
#include "../Resource/ShaderManager.h"
#include "../Resource/TextureManager.h"
#include "../Player/Player.h"

Renderer &Renderer::GetInstance()
{
    static Renderer __instance;
    return __instance;
}

Renderer::Renderer() : squareCount(0)
{
}

void Renderer::init()
{
    //square mesh
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   1.0f, 1.0f, // right top
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // right bottom
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left bottom
        -0.5f, 0.5f, 0.0f,  0.0f, 1.0f  // left top
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //create
    squareMesh.vao.create();
    squareMesh.vbo.create();
    squareMesh.ebo.create();

    //vertex vbo
    squareMesh.vbo.setData(sizeof(vertices), vertices);
    squareMesh.ebo.setData(sizeof(indices), indices);
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

    //render...
    squareShader.bind();
    squareTexture.bind();
    squareMesh.vao.bind();

    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, squareCount);

    squareMesh.vao.unbind();
    squareTexture.unbind();
    squareShader.unbind();
}

void Renderer::updateSquares(const std::vector<float> &vOffsets, const std::vector<glm::mat4> &matrices)
{
    squareMesh.instance_vbos[0].setData(vOffsets.size() * sizeof(float), &vOffsets[0]);
    squareMesh.instance_vbos[1].setData(matrices.size() * sizeof(glm::mat4), &matrices[0][0]);
    squareCount = matrices.size();
}