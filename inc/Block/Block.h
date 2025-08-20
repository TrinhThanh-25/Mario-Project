#ifndef MARIO_BLOCK_H
#define MARIO_BLOCK_H

#pragma once


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
	json saveToJson() const override;
	void loadFromJson(const json& j) override;
};

// ListMap.json
// j["ListMap"][0] = "Map1";
// j["ListMap"][1] = "MapCustom";
// j["ListMap"][2] = "NewMapCustom";

// testmap->setName("NewMap")

// NewMapCustom
#endif