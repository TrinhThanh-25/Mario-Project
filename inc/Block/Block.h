#ifndef BLOCK_H
#define BLOCK_H

#include "Common/Sprite.h"
#include "Mario/Mario.h"
#include "raylib.h"

class Block : public Sprite{
protected:
	bool hit = false;
	int earnedPoints;
public:
	Block() = default;
    Block(Vector2 pos, Vector2 size, Color color);
    Block(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
	Block(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, int earnedPoints);
    ~Block() override;

    void update() override = 0;
    void draw() override = 0;
	virtual void doHit(Mario& mario, Map* map);
	void resetHit();
};

#endif