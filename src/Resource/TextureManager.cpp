#include "TextureManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <sstream>

TextureManager &TextureManager::GetInstance() 
{
    static TextureManager __instance;
    return __instance;
}

TextureManager::TextureManager() 
{
}

void TextureManager::LoadTextures()
{
    //load images
    stbi_set_flip_vertically_on_load(true);
    uint8_t* ClayBlockImg = load_texture2d("./resources/textures/ClayBlock.png").data;
    uint8_t* GrassBlockSideImg = load_texture2d("./resources/textures/GrassBlockSide.png").data;
    uint8_t* GrassBlockTopImg = load_texture2d("./resources/textures/GrassBlockTop.png").data;
    uint8_t* StoneBlockImg = load_texture2d("./resources/textures/StoneBlock.png").data;
    uint8_t* SandBlockImg = load_texture2d("./resources/textures/SandBlock.png").data;
    uint8_t* SnowBlockSideImg = load_texture2d("./resources/textures/SnowBlockSide.png").data;
    uint8_t* SnowBlockTopImg = load_texture2d("./resources/textures/SnowBlockTop.png").data;
    
    //set data in opengl texture
    const uint32_t tex2d_size = tex2d_pxsize * tex2d_pxsize * 4;    //16(px) * 16(px) * 4(rgba)
    uint8_t tex2d_data[16 * 16 * 4 * 7];    //16(px) * 16(px) * 4(rgba) * 7(count)

    memcpy(tex2d_data, ClayBlockImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size, GrassBlockSideImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 2, GrassBlockTopImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 3, StoneBlockImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 4, SandBlockImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 5, SnowBlockSideImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 6, SnowBlockTopImg, tex2d_size);

    tex2d.create();
    tex2d.setData((GLubyte *)tex2d_data, tex2d_pxsize, tex2d_pxsize * tex2d_count, GL_TRUE);
    
    //free images
    stbi_image_free(ClayBlockImg);
    stbi_image_free(GrassBlockSideImg);
    stbi_image_free(GrassBlockTopImg);
    stbi_image_free(StoneBlockImg);
    stbi_image_free(SandBlockImg);
    stbi_image_free(SnowBlockSideImg);
    stbi_image_free(SnowBlockTopImg);

    //sun texture
    TexData sunTex = load_texture2d("./resources/textures/Sun.png");
    tex_sun.create();
    tex_sun.setData((GLubyte*)sunTex.data, sunTex.width, sunTex.height, GL_TRUE);

    //free images
    stbi_image_free(sunTex.data);

    //moon texture
    TexData moonTex = load_texture2d("./resources/textures/Moon.png");
    tex_moon.create();
    tex_moon.setData((GLubyte*)moonTex.data, moonTex.width, moonTex.height, GL_TRUE);

    //free images
    stbi_image_free(moonTex.data);
}

TextureManager::TexData TextureManager::load_texture2d(const std::string &filename) const
{
    GLsizei width, height, nrChannels;
    stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << filename << " texture load fail" << std::endl;
    }
    TexData tex;
    tex.data = data;
    tex.width = width;
    tex.height = height;
    tex.nrChannels = nrChannels;
    return tex;
}

uint32_t TextureManager::getTextureVOffset(TexureName tex) const
{
    return tex;
}
