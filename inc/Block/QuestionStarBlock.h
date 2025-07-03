#include "Block/Block.h"
#include "Game/Map.h"
#include "Mario/Mario.h"

class QuestionStarBlock : public Block {
public:
    QuestionStarBlock(Vector2 pos, Vector2 size, Color color);
	QuestionStarBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionStarBlock();

    void update() override;
    void draw() override;
    void doHit(Mario& mario, Map* map) override;

private:
    Item* item;
	float itemVelocityY;
	float itemMinY;
	Map* map;
};
