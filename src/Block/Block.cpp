#include "Block/Block.h"
#include "raylib.h"

Block::Block(Vector2 pos, Vector2 size, Color color)
    : Block(pos, size, color, 0, 1) {}

Block::Block(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
    : Sprite(pos, size, {0, 0}, color, frameTime, maxFrames, Direction::RIGHT) {}
	
Block::Block(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, int earnedPoints)
    : Sprite(pos, size, {0, 0}, color, frameTime, maxFrames, Direction::RIGHT), hit(false), earnedPoints(earnedPoints) {}

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