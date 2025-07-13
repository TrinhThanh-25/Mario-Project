#ifndef QUESTION_ONE_UP_MUSHROOM_BLOCK_H
#define QUESTION_ONE_UP_MUSHROOM_BLOCK_H

#include "Block/Block.h"
#include "Game/Map.h"
#include "Character/Character.h"

class QuestionOneUpMushroomBlock : public Block {
public:
    QuestionOneUpMushroomBlock(Vector2 pos, Vector2 size, Color color);
	QuestionOneUpMushroomBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionOneUpMushroomBlock();

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};

#endif // QUESTION_ONE_UP_MUSHROOM_BLOCK_H