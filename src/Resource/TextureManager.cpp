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
    uint8_t *ClayBlockImg = load_texture2d("./resources/textures/ClayBlock.png");
    uint8_t *GrassBlockSideImg = load_texture2d("./resources/textures/GrassBlockSide.png");
    uint8_t *GrassBlockTopImg = load_texture2d("./resources/textures/GrassBlockTop.png");
    uint8_t *StoneBlockImg = load_texture2d("./resources/textures/StoneBlock.png");
    
    //set data in opengl texture
    const uint32_t tex2d_size = tex2d_pxsize * tex2d_pxsize * 4;    //16(px) * 16(px) * 4(rgba)
    uint8_t tex2d_data[16 * 16 * 4 * 4];

    memcpy(tex2d_data, ClayBlockImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size, GrassBlockSideImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 2, GrassBlockTopImg, tex2d_size);
    memcpy(tex2d_data + tex2d_size * 3, StoneBlockImg, tex2d_size);

    tex2d.setData((GLubyte *)tex2d_data, tex2d_pxsize, tex2d_pxsize * tex2d_count, GL_TRUE);
    
    //free images
    stbi_image_free(ClayBlockImg);
    stbi_image_free(GrassBlockSideImg);
    stbi_image_free(GrassBlockTopImg);
    stbi_image_free(StoneBlockImg);
}

uint8_t* TextureManager::load_texture2d(const std::string &filename) const
{
    GLsizei width, height, nrChannels;
    stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cout << filename << " texture load fail" << std::endl;
    }
    return data;
}

uint32_t TextureManager::getTextureVOffset(TexureName tex) const
{
    return tex;
}
