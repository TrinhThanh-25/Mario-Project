#include "Block/ExclamationBlock.h"
#include "Block/Block.h"
#include "Game/World.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include "Game/Map.h"	
#include <string>

ExclamationBlock::ExclamationBlock( Vector2 pos, Vector2 size, Color color ) :
    ExclamationBlock( pos, size, color, 0.1, 4 ) {}

ExclamationBlock::ExclamationBlock( Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames ) :
    Block(BlockType::EXCLAMATIONBLOCK, pos, size, color, frameTime, maxFrames ),
    coinAnimationTime( 0.6f ),
    coinAnimationAcum( 0.0f ),
    coinAnimationFrame( 0 ),
    coinAnimationRunning( false ),
    coinY( 0.0f ),
    coinVelocityY( -400.0f ),
    stardustAnimationTime( 0.2f ),
    stardustAnimationAcum( 0.0f ),
    stardustAnimationFrame( 0 ),
    maxStardustAnimationFrame( 4 ),
    stardustAnimationRunning( false ),
    pointsFrameAcum( 0.0f ),
    pointsFrameTime( 0.5f ),
    pointsAnimationRunning( false ) {}

ExclamationBlock::~ExclamationBlock() = default;

void ExclamationBlock::update() {
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

void ExclamationBlock::draw() {
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
	} else {
		DrawTexture(ResourceManager::getTexture()["Exclamation0"],
			position.x, position.y, WHITE);
	}
}

void ExclamationBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		PlaySound(ResourceManager::getSound()["Coin"]);
		hit = true;
		coinAnimationRunning = true;
		character.getGameHud()->addPoints(earnedPoints);
		character.getGameHud()->addCoins(earnedPoints);
	}
}