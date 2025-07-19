#include "Block/Block.h"
#include "raylib.h"

Block::Block(BlockType blockType, Vector2 pos, Vector2 size, Color color)
    : Block(blockType, pos, size, color, 0, 1) {}

Block::Block(BlockType blockType, Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
    : Sprite(pos, size, {0, 0}, color, frameTime, maxFrames, Direction::RIGHT),
    blockType(blockType) {}
	
Block::Block(BlockType blockType, Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, int earnedPoints)
    : Sprite(pos, size, {0, 0}, color, frameTime, maxFrames, Direction::RIGHT), hit(false), earnedPoints(earnedPoints),
    blockType(blockType) {}

Block::~Block() = default;

void Block::update() {
}
void Block::draw() {
}
void Block::doHit(Character& character, Map* map) {

}

void Block::resetHit() {
    this->hit = false;
}

BlockType Block::getBlockType() {
    return blockType;
}
json Block::saveToJson() const {
	json j = Sprite::saveToJson();
	j["blockType"] = static_cast<int>(blockType);
	j["hit"] = hit;
	j["earnedPoints"] = earnedPoints;
	return j;
}

void Block::loadFromJson(const json& j) {
	Sprite::loadFromJson(j);
	blockType = static_cast<BlockType>(j["blockType"].get<int>());
	hit = j["hit"].get<bool>();
	earnedPoints = j["earnedPoints"].get<int>();
}