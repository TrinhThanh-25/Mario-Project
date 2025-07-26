#ifndef INVISIBLE_BLOCK_H
#define INVISIBLE_BLOCK_H

#include "Block/Block.h"
#include "Game/Map.h"
#include "Character/Character.h"

class InvisibleBlock : public Block {
private:
	
    float coinAnimationTime;
    float coinAnimationAcum;
    float coinFrameAcum;
    int coinAnimationFrame;
    bool coinAnimationRunning;
    float coinY;
    float coinVelocityY;

    float stardustAnimationTime;
    float stardustAnimationAcum;
    int stardustAnimationFrame;
    int maxStardustAnimationFrame;
    bool stardustAnimationRunning;

    float pointsFrameAcum;
    float pointsFrameTime;
    bool pointsAnimationRunning;

public: 
	InvisibleBlock(Vector2 pos, Vector2 size, Color color);
	InvisibleBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
	~InvisibleBlock() override;

	void update() override;
	void draw() override;

	void doHit(Character& character, Map* map) override;

    json saveToJson() const override;
    void loadFromJson(const json& j) override;
};
	
#endif