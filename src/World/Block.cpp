#include "Block.h"

std::array<Block::Textures, BLOCK_TYPE_NUM> Block::Texs;
std::array<bool, BLOCK_TYPE_NUM> Block::Transparency;

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
        Transparency[LogBlock] = false;
        Transparency[LeaveBlock] = true;
        Transparency[CactusBlock] = true;

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

        Texs[LogBlock].left = TextureManager::LogBlockSide;
        Texs[LogBlock].right = TextureManager::LogBlockSide;
        Texs[LogBlock].top = TextureManager::LogBlockTop;
        Texs[LogBlock].bottom = TextureManager::LogBlockTop;
        Texs[LogBlock].front = TextureManager::LogBlockSide;
        Texs[LogBlock].back = TextureManager::LogBlockSide;

        Texs[LeaveBlock].left = TextureManager::LeaveBlock;
        Texs[LeaveBlock].right = TextureManager::LeaveBlock;
        Texs[LeaveBlock].top = TextureManager::LeaveBlock;
        Texs[LeaveBlock].bottom = TextureManager::LeaveBlock;
        Texs[LeaveBlock].front = TextureManager::LeaveBlock;
        Texs[LeaveBlock].back = TextureManager::LeaveBlock;

        Texs[CactusBlock].left = TextureManager::CactusBlockSide;
        Texs[CactusBlock].right = TextureManager::CactusBlockSide;
        Texs[CactusBlock].top = TextureManager::CactusBlockTop;
        Texs[CactusBlock].bottom = TextureManager::CactusBlockBottom;
        Texs[CactusBlock].front = TextureManager::CactusBlockSide;
        Texs[CactusBlock].back = TextureManager::CactusBlockSide;

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