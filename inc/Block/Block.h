#ifndef BLOCK_H
#define BLOCK_H

#include "Common/Sprite.h"
#include "Character/Character.h"
#include "raylib.h"

enum class BlockType {
		CLOUDBLOCK,
		EXCLAMATIONBLOCK,
		EYESCLOSED,
		EYESOPENED,
		GLASSBLOCK,
		INVISIBLEBLOCK,
		MESSAGEBLOCK,
		STONEBLOCK,
		WOODBLOCK,
		QUESTIONBLOCK,
		QUESTIONFIREFLOWERBLOCK,
		QUESTIONMUSHROOMBLOCK,
		QUESTIONSTARBLOCK,
		QUESTIONTHREEUPMOONBLOCK,
		QUESTIONONEUPMUSHROOMBLOCK
	};

class Block : public Sprite{
protected:
	bool hit = false;
	int earnedPoints;
	BlockType blockType;
public:
	Block() = default;
    Block(BlockType blockType, Vector2 pos, Vector2 size, Color color);
    Block(BlockType blockType, Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
	Block(BlockType blockType, Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames, int earnedPoints);
    ~Block() override;

    void update() override = 0;
    void draw() override = 0;
	virtual void doHit(Character& character, Map* map);
	void resetHit();
	BlockType getBlockType();
};
#endif