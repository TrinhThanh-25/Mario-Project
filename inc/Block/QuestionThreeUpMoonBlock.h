#include "Block/Block.h"
#include "Game/Map.h"
#include "Character/Character.h"

class QuestionThreeUpMoonBlock : public Block {
public:
    QuestionThreeUpMoonBlock(Vector2 pos, Vector2 size, Color color);
	QuestionThreeUpMoonBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionThreeUpMoonBlock();

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};
