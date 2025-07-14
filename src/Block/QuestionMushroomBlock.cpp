#include "Block/QuestionMushroomBlock.h"
#include "Item/Mushroom.h"
#include "Block/Block.h"
#include "Character/Character.h"
#include "Game/World.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include "Game/Map.h"	

QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 size, Color color) :
	QuestionMushroomBlock(pos, size, color, 0.1f, 4) {}
QuestionMushroomBlock::QuestionMushroomBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames) :
	Block(pos, size, color, frameTime, maxFrames), item(nullptr), itemVelocityY(0.0f), itemMinY(0.0f), map(nullptr) {}
	
QuestionMushroomBlock::~QuestionMushroomBlock() {}

void QuestionMushroomBlock::update() {
	float deltaTime = GetFrameTime();
	if (hit && item) {
		item->setY(item->getY() + itemVelocityY * deltaTime);
		if (item->getY() < itemMinY) {
			item->setY(itemMinY);
			item->setState(SpriteState::ACTIVE);
			//map->getItems().push_back(item);
			item = nullptr; // Clear the item pointer after it has been released
			itemVelocityY = 0.0f;
		}
	}
	if (!hit) {
		frameAcum += deltaTime;
		if (frameAcum >= frameTime) {
			frameAcum = 0.0f;
			curFrame++;
			curFrame %= maxFrame;
		}
	}
}
void QuestionMushroomBlock::draw() {
	if (item) {
		item->draw();
	}
	if (hit) {
		DrawTexture(ResourceManager::getTexture()["EyesClosed0"], 
			position.x, position.y, WHITE);
	} else {
		DrawTexture(ResourceManager::getTexture()[std::string(TextFormat("Question%d", curFrame))],
			position.x, position.y, WHITE);
	}
}

void QuestionMushroomBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		hit = true;
		PlaySound(ResourceManager::getSound()["PowerUpAppear"]);
		item = new Mushroom(Vector2{ position.x, position.y }, Vector2{ 32, 32 }, Vector2(200, 0), RED, 0, true);
		itemVelocityY = -80.0f;
        item->setDirection(character.getDirection());
		itemMinY = position.y - 32.0f; // Set the minimum Y position for the item
		this->map = map; // Store the map reference
	}
}