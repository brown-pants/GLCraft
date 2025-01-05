#include "Block.h"

std::array<Block::Textures, 6> Block::Texs;
std::array<bool, 6> Block::Transparency;

Block::Block(Block_Type type) : block_type(type)
{
    static bool isInited = false;
    if(!isInited)
    {
        Transparency[Air] = true;
        Transparency[ClayBlock] = false;
        Transparency[GrassBlock] = false;
        Transparency[StoneBlock] = false;
        Transparency[SandBlock] = false;
        Transparency[SnowBlock] = false;

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

        Texs[SandBlock].left = TextureManager::SandBlock;
        Texs[SandBlock].right = TextureManager::SandBlock;
        Texs[SandBlock].top = TextureManager::SandBlock;
        Texs[SandBlock].bottom = TextureManager::SandBlock;
        Texs[SandBlock].front = TextureManager::SandBlock;
        Texs[SandBlock].back = TextureManager::SandBlock;

        Texs[SnowBlock].left = TextureManager::SnowBlockSide;
        Texs[SnowBlock].right = TextureManager::SnowBlockSide;
        Texs[SnowBlock].top = TextureManager::SnowBlockTop;
        Texs[SnowBlock].bottom = TextureManager::ClayBlock;
        Texs[SnowBlock].front = TextureManager::SnowBlockSide;
        Texs[SnowBlock].back = TextureManager::SnowBlockSide;

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