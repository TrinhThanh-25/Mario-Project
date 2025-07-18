#include "Block/WoodBlock.h"
#include "Common/ResourceManager.h"

WoodBlock::WoodBlock(Vector2 pos, Vector2 size, Color color)
	: WoodBlock(pos, size, color, 0, 1) {}

WoodBlock::WoodBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
	: Block(BlockType::WOODBLOCK, pos, size, color, frameTime, maxFrames) {}

WoodBlock::~WoodBlock() = default;

void WoodBlock::draw() {
    DrawTexture(
        ResourceManager::getTexture()["Wood0"],
        position.x,
        position.y,
        WHITE
    );
}

void WoodBlock::update() {
}