#ifndef QUESTION_ONE_UP_MUSHROOM_BLOCK_H
#define QUESTION_ONE_UP_MUSHROOM_BLOCK_H

#include "Block/Block.h"
#include "Character/Character.h"

class Map;
class Item;

class QuestionOneUpMushroomBlock : public Block {
public:
    QuestionOneUpMushroomBlock(Vector2 pos, Vector2 size, Color color);
	QuestionOneUpMushroomBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionOneUpMushroomBlock();

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

    json saveToJson() const override;
    void loadFromJson(const json& j) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};

#endif