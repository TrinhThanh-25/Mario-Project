#ifndef WOOD_BLOCK_H
#define WOOD_BLOCK_H

#include "Block/Block.h"

class WoodBlock : public Block {
public:
    WoodBlock(Vector2 pos, Vector2 size, Color color);
    WoodBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~WoodBlock();

    void draw() override;
    void update() override;
};

#endif