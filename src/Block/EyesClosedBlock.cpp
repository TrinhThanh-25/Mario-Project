#include "Block/Block.h"
#include "Block/EyesClosedBlock.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include <string>

EyesClosedBlock::EyesClosedBlock(Vector2 pos, Vector2 size, Color color) :
	EyesClosedBlock(pos, size, color, 0.1f, 4) {}

EyesClosedBlock::EyesClosedBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames) :
	Block(pos, size, color, frameTime, maxFrames) {}

EyesClosedBlock::~EyesClosedBlock() = default;

void EyesClosedBlock::update() {}

void EyesClosedBlock::draw() {
	DrawTexture(ResourceManager::getTexture()["EyesClosed0"], position.x, position.y, WHITE);
}
