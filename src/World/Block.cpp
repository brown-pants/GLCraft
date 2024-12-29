#include "Block.h"

std::array<Block::Textures, 4> Block::Texs;
std::array<bool, 4> Block::Transparency;

Block::Block(Block_Type type) : block_type(type)
{
    static bool isInited = false;
    if(!isInited)
    {
        Transparency[Air] = true;
        Transparency[ClayBlock] = false;
        Transparency[GrassBlock] = false;
        Transparency[StoneBlock] = false;

        Texs[ClayBlock].left = TextureManager::ClayBlock;
        Texs[ClayBlock].right = TextureManager::ClayBlock;
        Texs[ClayBlock].top = TextureManager::ClayBlock;
        Texs[ClayBlock].bottom = TextureManager::ClayBlock;
        Texs[ClayBlock].front = TextureManager::ClayBlock;
        Texs[ClayBlock].back = TextureManager::ClayBlock;

        Texs[GrassBlock].left = TextureManager::GrassBlockSide;
        Texs[GrassBlock].right = TextureManager::GrassBlockSide;
        Texs[GrassBlock].top = TextureManager::GrassBlockTop;
        Texs[GrassBlock].bottom = TextureManager::ClayBlock;
        Texs[GrassBlock].front = TextureManager::GrassBlockSide;
        Texs[GrassBlock].back = TextureManager::GrassBlockSide;

        Texs[StoneBlock].left = TextureManager::StoneBlock;
        Texs[StoneBlock].right = TextureManager::StoneBlock;
        Texs[StoneBlock].top = TextureManager::StoneBlock;
        Texs[StoneBlock].bottom = TextureManager::StoneBlock;
        Texs[StoneBlock].front = TextureManager::StoneBlock;
        Texs[StoneBlock].back = TextureManager::StoneBlock;

        isInited = true;
    }
}

bool Block::isTransparent() const
{
    return Transparency[block_type];
}

TextureManager::TexureName Block::getLeftTexture() const
{
    return Texs[block_type].left;
}

TextureManager::TexureName Block::getRightTexture() const
{
    return Texs[block_type].right;
}

TextureManager::TexureName Block::getTopTexture() const
{
    return Texs[block_type].top;
}

TextureManager::TexureName Block::getBottomTexture() const
{
    return Texs[block_type].bottom;
}

TextureManager::TexureName Block::getFrontTexture() const
{
    return Texs[block_type].front;
}

TextureManager::TexureName Block::getBackTexture() const
{
    return Texs[block_type].back;
}