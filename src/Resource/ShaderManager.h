#ifndef __SHADERMANAGER_H__
#define __SHADERMANAGER_H__
#include "../GL/GLShader.h"
#include <array>
#include <string>

class ShaderManager
{
public:
    enum ShaderName
    {
        Square = 0,
        Crosshair,
        Planet,
        Texture,
        Water
    };
    static ShaderManager &GetInstance();
    void LoadShaders();
    GLShader &getShader(ShaderName name) { return shaders[name]; }

private:
    std::array<GLShader, 5> shaders;
    GLShader load_shader(const std::string &vs_filename, const std::string &fs_filename);

    ShaderManager();
};
#endif