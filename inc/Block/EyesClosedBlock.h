#ifndef EYES_CLOSED_BLOCK_H
#define EYES_CLOSED_BLOCK_H

#include "Block/Block.h"

class EyesClosedBlock : public Block {
public:
    EyesClosedBlock(Vector2 pos, Vector2 size, Color color);
	EyesClosedBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~EyesClosedBlock() override;

    void update() override;
    void draw() override;
};

#endif 