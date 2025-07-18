#include "Block/QuestionBlock.h"
#include "Game/Map.h"
#include "Common/ResourceManager.h"
#include "Character/Character.h"
#include "Block/Block.h"
#include <string>
#include "Game/World.h"
#include "raylib.h"
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 size, Color color)
	: QuestionBlock(pos, size, color, 0.1, 4) {}
	
QuestionBlock::QuestionBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames)
	: Block(BlockType::QUESTIONBLOCK, pos, size, color, frameTime, maxFrames),
	coinAnimationTime(0.6),
	coinAnimationAcum(0),
	coinFrameAcum(0),
	coinAnimationFrame(0),
	coinAnimationRunning(false),
	coinY(0),
	coinVelocityY(-400),
	stardustAnimationTime(0.2),
	stardustAnimationAcum(0),
	stardustAnimationFrame(0),
	maxStardustAnimationFrame(4),
	stardustAnimationRunning(false),
	pointsFrameAcum(0),
	pointsFrameTime(0.5),
	pointsAnimationRunning(false) {}
	
QuestionBlock::~QuestionBlock() = default;

void QuestionBlock::update() {
	
	const float deltaTime = GetFrameTime();
	
	if (!hit) {
		frameAcum += deltaTime;
		if (frameAcum >= frameTime) {
			frameAcum = 0;
			curFrame++;
			if (curFrame >= maxFrame) {
				curFrame = 0;
			}
		}
	}
	
	if (hit && coinAnimationRunning) {
		coinAnimationAcum += deltaTime;
		if (coinAnimationAcum >= coinAnimationTime) {
			coinAnimationRunning = false;
			stardustAnimationRunning = true;
			pointsAnimationRunning = true;
			coinAnimationFrame++;
			coinAnimationFrame %= maxFrame;
        }	
			coinFrameAcum += deltaTime;
			if (coinFrameAcum >= frameTime) {
				coinFrameAcum = 0;
				coinAnimationFrame++;
				coinAnimationFrame %= maxFrame;
			}
			coinY += coinVelocityY * deltaTime;

			coinVelocityY += World::gravity;
		}
	
	if (stardustAnimationRunning) {
		stardustAnimationAcum += deltaTime;
		if (stardustAnimationAcum >= stardustAnimationTime) {
			stardustAnimationAcum = 0;
			stardustAnimationFrame++;
			if (stardustAnimationFrame >= maxStardustAnimationFrame) {
				stardustAnimationRunning = false;
			}
		}
	}
	if( pointsAnimationRunning) {
		pointsFrameAcum += deltaTime;
		if (pointsFrameAcum >= pointsFrameTime) {
			pointsFrameAcum = 0;
			pointsAnimationRunning = false;
		}
	}
}

void QuestionBlock::draw() {
	if (coinAnimationRunning) {
		DrawTexture(ResourceManager::getTexture()["Coin" + std::to_string(coinAnimationFrame)],
			position.x + 4, position.y + coinY, WHITE);
	}
	//if (stardustAnimationRunning) {
	//	DrawTexture(ResourceManager::getTexture()["Stardust" + std::to_string(stardustAnimationFrame)],
	//		position.x, position.y - size.y, WHITE);
	//}	//chua co stardust texture
	if (pointsAnimationRunning) {
		std::string pointsTexture = "Gui" + std::to_string(earnedPoints);
		Texture2D& texture = ResourceManager::getTexture()[pointsTexture];
		DrawTexture(texture,
			static_cast<int>(position.x + size.x / 2 - texture.width / 2),
			static_cast<int>(position.y - size.y / 2 - texture.height - (20 * pointsFrameAcum / pointsFrameTime)),
			WHITE);
	}
	if (hit) {
		DrawTexture(ResourceManager::getTexture()["EyesClosed0"], position.x, position.y, WHITE);
	}
	else {
		DrawTexture(ResourceManager::getTexture()["Question" + std::to_string(curFrame)], position.x, position.y, WHITE);
	}
}

void QuestionBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		PlaySound(ResourceManager::getSound()["Coin"]);
		hit = true;
		coinAnimationRunning = true;
		coinY = position.y;
		character.getGameHud()->addPoints(earnedPoints);
		character.getGameHud()->addCoins(earnedPoints);
	}
}
json QuestionBlock::saveToJson() const {
	json j = Block::saveToJson();
	j["coinAnimationTime"] = coinAnimationTime;
	j["coinAnimationAcum"] = coinAnimationAcum;
	j["coinFrameAcum"] = coinFrameAcum;
	j["coinAnimationFrame"] = coinAnimationFrame;
	j["coinAnimationRunning"] = coinAnimationRunning;
	j["coinY"] = coinY;
	j["coinVelocityY"] = coinVelocityY;
	j["stardustAnimationTime"] = stardustAnimationTime;
	j["stardustAnimationAcum"] = stardustAnimationAcum;
	j["stardustAnimationFrame"] = stardustAnimationFrame;
	j["maxStardustAnimationFrame"] = maxStardustAnimationFrame;
	j["stardustAnimationRunning"] = stardustAnimationRunning;
	j["pointsFrameAcum"] = pointsFrameAcum;
	j["pointsFrameTime"] = pointsFrameTime;
	j["pointsAnimationRunning"] = pointsAnimationRunning;
	return j;
}
void QuestionBlock::loadFromJson(const json& j) {
	Block::loadFromJson(j);
	coinAnimationTime = j["coinAnimationTime"].get<float>();
	coinAnimationAcum = j["coinAnimationAcum"].get<float>();
	coinFrameAcum = j["coinFrameAcum"].get<float>();
	coinAnimationFrame = j["coinAnimationFrame"].get<int>();
	coinAnimationRunning = j["coinAnimationRunning"].get<bool>();
	coinY = j["coinY"].get<float>();
	coinVelocityY = j["coinVelocityY"].get<float>();
	stardustAnimationTime = j["stardustAnimationTime"].get<float>();
	stardustAnimationAcum = j["stardustAnimationAcum"].get<float>();
	stardustAnimationFrame = j["stardustAnimationFrame"].get<int>();
	maxStardustAnimationFrame = j["maxStardustAnimationFrame"].get<int>();
	stardustAnimationRunning = j["stardustAnimationRunning"].get<bool>();
	pointsFrameAcum = j["pointsFrameAcum"].get<float>();
	pointsFrameTime = j["pointsFrameTime"].get<float>();
	pointsAnimationRunning = j["pointsAnimationRunning"].get<bool>();
}