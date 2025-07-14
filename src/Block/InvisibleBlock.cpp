#include "Block/InvisibleBlock.h"
#include "Block/Block.h"
#include "Character/Character.h"
#include "Game/World.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include "Game/Map.h"	
#include <string>

InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 size, Color color) :
	InvisibleBlock(pos, size, color, 0.1f, 4) {}
			
InvisibleBlock::InvisibleBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames) :
	Block(pos, size, color, frameTime, maxFrames, 10),
	coinAnimationTime(0.6f),
	coinAnimationAcum(0.0f),
	coinFrameAcum(0.0f),
	coinAnimationFrame(0),
	coinAnimationRunning(false),
	coinY(0.0f),
	coinVelocityY(-400.0f),
	stardustAnimationTime(0.2f),
	stardustAnimationAcum(0.0f),
	stardustAnimationFrame(0),
	maxStardustAnimationFrame(4),
	stardustAnimationRunning(false),
	pointsFrameAcum(0.0f),
	pointsFrameTime(0.5f),
	pointsAnimationRunning(false) {
	setState(SpriteState::INVISIBLE);
}

InvisibleBlock::~InvisibleBlock() = default;

void InvisibleBlock::update() {
	float deltaTime = GetFrameTime();
	if (hit && coinAnimationRunning) {
		coinAnimationAcum += deltaTime;
		frameAcum += deltaTime;

		if (coinAnimationAcum >= coinAnimationTime) {
			coinAnimationRunning = false;
			stardustAnimationRunning = true;
			pointsAnimationRunning = true;
		}

		if (frameAcum >= frameTime) {
			frameAcum = 0.0f;
			coinAnimationFrame++;
			coinAnimationFrame %= maxFrame;
		}

		coinY += coinVelocityY * deltaTime;
		coinVelocityY += World::gravity;
		if (coinY < -100.0f) {
			coinY = -100.0f;
			coinVelocityY = 0.0f;
		}
	}

	if (stardustAnimationRunning) {
		stardustAnimationAcum += deltaTime;
		if (stardustAnimationAcum >= stardustAnimationTime) {
			stardustAnimationAcum = 0.0f;
			stardustAnimationFrame++;
			if (stardustAnimationFrame >= maxStardustAnimationFrame) {
				stardustAnimationFrame = 0;
				stardustAnimationRunning = false;
			}
		}
	}

	if (pointsAnimationRunning) {
		pointsFrameAcum += deltaTime;
		if (pointsFrameAcum >= pointsFrameTime) {
			pointsFrameAcum = 0.0f;
			pointsAnimationRunning = false;
		}
	}
}

void InvisibleBlock::draw() {
	if (coinAnimationRunning) {
		std::string coinTexture = "Coin" + std::to_string(coinAnimationFrame);
		DrawTexture(ResourceManager::getTexture()[coinTexture],
			position.x + 4, coinY, WHITE);
	}

	if (stardustAnimationRunning) {
		std::string stardustTexture = "Stardust" + std::to_string(stardustAnimationFrame);
		DrawTexture(ResourceManager::getTexture()[stardustTexture],
			position.x, position.y - size.y, WHITE);
	}

	if (pointsAnimationRunning) {
		DrawTexture(ResourceManager::getTexture()["Points"],
			position.x + size.x/2 - ResourceManager::getTexture()["Points"].width / 2,
			position.y - size.y/2 - ResourceManager::getTexture()["Points"].height - ( 20 * pointsFrameAcum / pointsFrameTime ), WHITE);
	}
	if (hit) {
		DrawTexture(ResourceManager::getTexture()["EyesClosed0"],
			position.x, position.y, WHITE);
	} 
}

void InvisibleBlock::doHit(Character& character, Map *map) {
    if ( !hit ) {
        PlaySound( ResourceManager::getSound()["Coin"] );
        hit = true;
        coinAnimationRunning = true;
		coinY = position.y;
		character.addPoints(earnedPoints);
		character.addCoin(earnedPoints);
		setState(SpriteState::IDLE); // Set the block to idle state after being hit
    }
}