#ifndef EXCLAMATIONBLOCK_H
#define EXCLAMATIONBLOCK_H

#include "Block/Block.h"

class ExclamationBlock : public Block {
private:
	float coinAnimationTime;
    float coinAnimationAcum;
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
    ExclamationBlock(Vector2 pos, Vector2 size, Color color);
	ExclamationBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~ExclamationBlock() override;

    void update() override;
    void draw() override;
    void doHit(Character& character, Map* map) override;

    json saveToJson() const override;
    void loadFromJson(const json& j) override;
};

#endif