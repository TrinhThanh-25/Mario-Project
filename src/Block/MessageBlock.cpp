#include "Block/Block.h"
#include "Block/MessageBlock.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include <string>

MessageBlock::MessageBlock(Vector2 pos, Vector2 size, Color color, std::string message)
	: MessageBlock(pos, size, color, 0, 1, std::move(message)) {}

MessageBlock::MessageBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, std::string message)
	: Block(pos, size, color, frameTime, maxFrames), message(std::move(message)),
	moveTime(0.2f), moveAcum(0.0f), moveY(0.0f), hasMoved(false), showMessage(false), messageTimer(2.0f) {}

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
	if (showMessage) {
		messageTimer -= deltaTime;
		if (messageTimer <= 0.0f) {
			showMessage = false; // Hide the message after the timer expires
			messageTimer = 2.0f; // Reset the timer for future hits
		}
	}
}
void MessageBlock::draw() {
	DrawTexture(ResourceManager::getTexture()["Message0"], position.x, position.y, WHITE);

	// Draw the message if showMessage is true
	if (showMessage) {
		// Draw a simple background rectangle for the message
		int msgWidth = MeasureText(message.c_str(), 20) + 20;
		int msgHeight = 40;
		int msgX = static_cast<int>(position.x + size.x / 2 - msgWidth / 2);
		int msgY = static_cast<int>(position.y - msgHeight - 10);

		DrawRectangle(msgX, msgY, msgWidth, msgHeight, Fade(BLACK, 0.7f));
		DrawText(message.c_str(), msgX + 10, msgY + 10, 20, YELLOW);
	}
}

void MessageBlock::doHit(Mario& mario, Map* map) {
	if (!hit) {
		hit = true;
		hasMoved = true;
		PlaySound(ResourceManager::getSound()["MessageBlock"]);
		showMessage = true;
		messageTimer = 2.0f; // Show message for 2 seconds
	}
}
