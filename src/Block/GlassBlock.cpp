#include "Block/GlassBlock.h"
#include "Common/ResourceManager.h"

GlassBlock::GlassBlock(Vector2 pos, Vector2 size, Color color)
	: GlassBlock(pos, size, color, 0, 1) {}

GlassBlock::GlassBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
 : Block(BlockType::GLASSBLOCK, pos, size, color, frameTime, maxFrames) {}

GlassBlock::~GlassBlock() {}

void GlassBlock::update() {
}

void GlassBlock::draw() {
	DrawTexture(
		ResourceManager::getTexture()["Glass0"],
		position.x,
		position.y,
		WHITE
	);

}