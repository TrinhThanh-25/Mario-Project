#ifndef QUESTIONFIREFLOWERBLOCK_H
#define QUESTIONFIREFLOWERBLOCK_H

#include "Block/Block.h"
#include "Game/Map.h"
#include "Character/Character.h"

class QuestionFireFlowerBlock : public Block {
public:
    QuestionFireFlowerBlock(Vector2 pos, Vector2 size, Color color);
	QuestionFireFlowerBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionFireFlowerBlock();

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};

#endif // QUESTIONFIREFLOWERBLOCK_H