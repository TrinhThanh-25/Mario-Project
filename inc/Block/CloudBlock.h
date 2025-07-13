#ifndef CLOUD_BLOCK_H
#define CLOUD_BLOCK_H

#include "Block/Block.h"

class CloudBlock : public Block {
public:
    CloudBlock(Vector2 pos, Vector2 size, Color color);
    CloudBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~CloudBlock() override;

    void update() override;
    void draw() override;
};

#endif