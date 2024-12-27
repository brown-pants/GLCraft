#ifndef __TEXTUREMANAGER_H__
#define __TEXTUREMANAGER_H__
#include "../GL/GLTextures.h"
#include <string>

class TextureManager
{
public:
    enum TexureName 
    {
        ClayBlock = 0,
        GrassBlockSide,
        GrassBlockTop,
        StoneBlock
    };
    static TextureManager &GetInstance();
    void LoadTextures();
    GLTexture2D &getTexture2d() { return tex2d; }
    uint32_t getTextureVOffset(TexureName tex) const;

    const uint32_t tex2d_pxsize = 16;   //textures pixmap : 16 * 16
    const uint32_t tex2d_count = 4;     //textures count
    
private:
    GLTexture2D tex2d;

    TextureManager();
    uint8_t* load_texture2d(const std::string &filename) const;
};
#endif