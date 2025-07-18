#include "Block/QuestionFireFlowerBlock.h"
#include "Block/Block.h"
#include "Character/Character.h"
#include "Game/World.h"
#include "Item/ItemFactory.h"
#include "raylib.h"
#include "Common/ResourceManager.h"
#include "Game/Map.h"	

QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 size, Color color) :
	QuestionFireFlowerBlock(pos, size, color, 0.1f, 4) {}
	
QuestionFireFlowerBlock::QuestionFireFlowerBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames) :
	Block(pos, size, color, frameTime, maxFrames), item(nullptr), itemVelocityY(0.0f), itemMinY(0.0f), map(nullptr) {}

QuestionFireFlowerBlock::~QuestionFireFlowerBlock() {}

void QuestionFireFlowerBlock::update() {
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
void QuestionFireFlowerBlock::draw() {
	if (item) {
		item->draw();
	}
	if (hit) {
		DrawTexture(ResourceManager::getTexture()["EyesClosed0"], 
			position.x, position.y, WHITE);
	}
	else {
		DrawTexture(ResourceManager::getTexture()[std::string(TextFormat("Question%d", curFrame))],
			position.x, position.y, WHITE);
	}
}
void QuestionFireFlowerBlock::doHit(Character& character, Map* map) {
	if (!hit) {
		hit = true;
		PlaySound(ResourceManager::getSound()["PowerUpAppear"]);
		item = ItemFactory::createItem(ItemType::FLOWER, Source::BLOCK, Vector2{ position.x, position.y }, character.getDirection());
		itemVelocityY = -80.0f;
        item->setDirection(character.getDirection());
		itemMinY = position.y - 32.0f; // Set the minimum Y position for the item
		this->map = map; // Store the map reference
	}
}