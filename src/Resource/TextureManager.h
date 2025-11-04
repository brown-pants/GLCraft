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
        StoneBlock,
        SandBlock,
        SnowBlockSide,
        SnowBlockTop,
        LogBlockTop,
        LogBlockSide,
        LeaveBlock,
        CactusBlockTop,
        CactusBlockSide,
        CactusBlockBottom
    };
    static TextureManager &GetInstance();
    void LoadTextures();
    GLTexture2D &getTexture2d() { return tex2d; }
    uint32_t getTextureVOffset(TexureName tex) const;

    GLTexture2D& getSunTexture() { return tex_sun; }
    GLTexture2D& getMoonTexture() { return tex_moon; }
    GLTexture2D& getLoadingTexture() { return tex_loading; }
    GLTexture2D& getWaterMap() { return water_map; }

    const uint32_t tex2d_pxsize = 16;   //textures pixmap : 16 * 16
    const uint32_t tex2d_count = 13;     //textures count
    
private:
    GLTexture2D tex2d;
    GLTexture2D tex_sun, tex_moon, tex_loading;
    GLTexture2D water_map;

    TextureManager();
    struct TexData
    {
        uint8_t* data;
        int width;
        int height;
        int nrChannels;
    };
    TexData load_texture2d(const std::string &filename) const;
};
#endif