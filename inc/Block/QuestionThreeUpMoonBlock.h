#ifndef QUESTIONTHREEUPMOONBLOCK_H
#define QUESTIONTHREEUPMOONBLOCK_H

#include "Block/Block.h"
#include "Character/Character.h"

class Map;
class Item;

class QuestionThreeUpMoonBlock : public Block {
public:
    QuestionThreeUpMoonBlock(Vector2 pos, Vector2 size, Color color);
	QuestionThreeUpMoonBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionThreeUpMoonBlock();

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