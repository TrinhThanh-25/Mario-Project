#include "Block/StoneBlock.h"
#include "Common/ResourceManager.h"
StoneBlock::StoneBlock(Vector2 pos, Vector2 size, Color color)
    : Block(pos, size, color) {}

StoneBlock::StoneBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
    : Block(pos, size, color, frameTime, maxFrames) {}

StoneBlock::~StoneBlock() {}

void StoneBlock::draw() {
	DrawTexture(
		ResourceManager::getTexture()["Stone0"],
		position.x,
		position.y,
		WHITE
	);
	}

void StoneBlock::update() {
}
