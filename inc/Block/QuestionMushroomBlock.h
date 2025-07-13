#ifndef QUESTION_MUSHROOM_BLOCK_H
#define QUESTION_MUSHROOM_BLOCK_H

#include "Block/Block.h"
#include "Character/Character.h"

class Map;
class Item;

class QuestionMushroomBlock : public Block {
public:
    QuestionMushroomBlock(Vector2 pos, Vector2 size, Color color);
	QuestionMushroomBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionMushroomBlock();

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};

#endif 