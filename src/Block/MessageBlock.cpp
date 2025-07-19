#include "Block/Block.h"
#include "Block/MessageBlock.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include <string>

MessageBlock::MessageBlock(Vector2 pos, Vector2 size, Color color, std::string message)
	: MessageBlock(pos, size, color, 0, 1, std::move(message)) {}

MessageBlock::MessageBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, std::string message)
	: Block(BlockType::MESSAGEBLOCK, pos, size, color, frameTime, maxFrames), message(std::move(message)),
	moveTime(0.2f), moveAcum(0.0f), moveY(0.0f), hasMoved(false) {}

MessageBlock::~MessageBlock() = default;

void MessageBlock::update() {
	const float deltaTime = GetFrameTime();
	if (hasMoved) {
		moveAcum += deltaTime;
		if (moveAcum >= moveTime) {
			moveY = 0;
			hasMoved = false;
			moveAcum = 0.0f; 
		} else {
			moveY += 100.0f * deltaTime; 
		}
		position.y -= moveY; // Apply the vertical movement
	}

}
void MessageBlock::draw() {
	DrawTexture(ResourceManager::getTexture()["Message0"], position.x, position.y, WHITE);
}

void MessageBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		hit = true;
		hasMoved = true;
		PlaySound(ResourceManager::getSound()["MessageBlock"]);
		map->showMessage();
	}
}
json MessageBlock::saveToJson() const {
	json j = Block::saveToJson();
	j["message"] = message;
	j["moveTime"] = moveTime;
	j["moveAcum"] = moveAcum;
	j["moveY"] = moveY;
	j["hasMoved"] = hasMoved;
	return j;
}

void MessageBlock::loadFromJson(const json& j) {
	Block::loadFromJson(j);
	message = j["message"].get<std::string>();
	moveTime = j["moveTime"].get<float>();
	moveAcum = j["moveAcum"].get<float>();
	moveY = j["moveY"].get<float>();
	hasMoved = j["hasMoved"].get<bool>();
}
