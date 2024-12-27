#ifndef __BLOCK_H__
#define __BLOCK_H__
#include <array>
#include "../Resource/TextureManager.h"

enum Block_Type
{
    Air = 0,
    ClayBlock,
    GrassBlock,
    StoneBlock
};

class Block
{
public:
    Block(Block_Type type = Air);
    bool isTransparent() const;

    void operator=(Block_Type type)
    {
        block_type = type;
    }

    TextureManager::TexureName getLeftTexture();
    TextureManager::TexureName getRightTexture();
    TextureManager::TexureName getTopTexture();
    TextureManager::TexureName getBottomTexture();
    TextureManager::TexureName getFrontTexture();
    TextureManager::TexureName getBackTexture();

private:
    struct Textures
    {
        TextureManager::TexureName left, right, top, bottom, front, back;
    };
    static std::array<Textures, 4> Texs;
    static std::array<bool, 4> Transparency;
    Block_Type block_type;
};

#endif