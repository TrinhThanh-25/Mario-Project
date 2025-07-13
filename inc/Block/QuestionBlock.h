#ifndef QUESTIONBLOCK_H
#define QUESTIONBLOCK_H

#include "Block/Block.h"
#include "Character/Character.h"
#include "Game/Map.h"

class QuestionBlock : public Block {
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
    QuestionBlock(Vector2 pos, Vector2 size, Color color);
    QuestionBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~QuestionBlock() override;

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;
};

#endif