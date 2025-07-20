#include "Block/EyesOpenedBlock.h"
#include "Game/Map.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include "Block/Block.h"
#include <string>

EyesOpenedBlock::EyesOpenedBlock(Vector2 pos, Vector2 size, Color color)
	: EyesOpenedBlock(pos, size, color, 0.1, 1) {}
EyesOpenedBlock::EyesOpenedBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
	: Block(BlockType::EYESOPENED, pos, size, color, frameTime, maxFrames), animationTime(3), animationAccumulated(0) {}

EyesOpenedBlock::~EyesOpenedBlock() = default;

void EyesOpenedBlock::update() {
	const float deltaTime = GetFrameTime();
	frameAcum += deltaTime;
	if (frameAcum >= frameTime) {
		frameAcum = 0;
		curFrame = (curFrame + 1) % maxFrame;
	}
	if (hit) {
		animationAccumulated += deltaTime;
		if (animationAccumulated >= animationTime) {
			hit = false;
			animationAccumulated = 0;
			curFrame = 0;
			state = SpriteState::IDLE;
		}
	}
}
void EyesOpenedBlock::draw() {
	if (hit) {
		DrawTexture(ResourceManager::getTexture()[std::string( TextFormat( "EyesOpened%d", curFrame))], position.x, position.y, WHITE);
	} else {
		DrawTexture(ResourceManager::getTexture()["EyesOpened0"], position.x, position.y, WHITE);
	}
}

void EyesOpenedBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		PlaySound(ResourceManager::getSound()["ShellRicochet"]);
		hit = true;
		state = SpriteState::HIT;
	}
}

json EyesOpenedBlock::saveToJson() const {
	json j = Block::saveToJson();
	j["animationTime"] = animationTime;
	j["animationAccumulated"] = animationAccumulated;
	return j;
}

void EyesOpenedBlock::loadFromJson(const json& j) {
	Block::loadFromJson(j);
	animationTime = j["animationTime"].get<float>();
	animationAccumulated = j["animationAccumulated"].get<float>();
}