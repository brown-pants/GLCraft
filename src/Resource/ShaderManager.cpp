#include "ShaderManager.h"

ShaderManager &ShaderManager::GetInstance()
{
    static ShaderManager __instance;
    return __instance;
}

ShaderManager::ShaderManager() 
{
}

void ShaderManager::LoadShaders()
{
    shaders[Square] = load_shader("./resources/shaders/Square.vs", "./resources/shaders/Square.fs");
    shaders[Crosshair] = load_shader("./resources/shaders/Crosshair.vs", "./resources/shaders/Crosshair.fs");
    shaders[Planet] = load_shader("./resources/shaders/Planet.vs", "./resources/shaders/Planet.fs");
}

GLShader ShaderManager::load_shader(const std::string &vs_filename, const std::string &fs_filename)
{
    std::string vertexCode;
    std::string fragmentCode;
    try
    {
        std::ifstream vertexShaderFile(vs_filename);
        std::ifstream fragmentShaderFile(fs_filename);
        std::stringstream vShaderStream, fShaderStream;

        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();

        vertexShaderFile.close();
        fragmentShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    
    GLShader shader;
    shader.create();
    shader.compile(vShaderCode, fShaderCode);
    
    return shader;
}