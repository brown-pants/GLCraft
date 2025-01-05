#ifndef __BLOCK_H__
#define __BLOCK_H__
#include <array>
#include "../Resource/TextureManager.h"

enum Block_Type
{
    Air = 0,
    ClayBlock,
    GrassBlock,
    StoneBlock,
    SandBlock,
    SnowBlock
};

class Block
{
public:
    Block(Block_Type type = Air);
    bool isTransparent() const;
    inline Block_Type type() const { return block_type; }

    void operator=(Block_Type type)
    {
        block_type = type;
    }

    TextureManager::TexureName getLeftTexture() const;
    TextureManager::TexureName getRightTexture() const;
    TextureManager::TexureName getTopTexture() const;
    TextureManager::TexureName getBottomTexture() const;
    TextureManager::TexureName getFrontTexture() const;
    TextureManager::TexureName getBackTexture() const;

private:
    struct Textures
    {
        TextureManager::TexureName left, right, top, bottom, front, back;
    };
    static std::array<Textures, 6> Texs;
    static std::array<bool, 6> Transparency;
    Block_Type block_type;
};

#endif