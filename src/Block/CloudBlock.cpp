#include "Block/Block.h"
#include "Block/CloudBlock.h"
#include "raylib.h"
#include "Common/ResourceManager.h"

CloudBlock::CloudBlock(Vector2 pos, Vector2 size, Color color)
    : CloudBlock(pos, size, color, 0, 1) {
    setState(SpriteState::SOLID_ABOVE);
}

CloudBlock::CloudBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
    : Block(BlockType::CLOUDBLOCK, pos, size, color, frameTime, maxFrames) {
    setState(SpriteState::SOLID_ABOVE);
}

CloudBlock::~CloudBlock() = default;

void CloudBlock::draw() {
    DrawTexture(
        ResourceManager::getTexture()["Cloud0"],
        position.x,
        position.y,
        WHITE
    );
}

void CloudBlock::update() {
}