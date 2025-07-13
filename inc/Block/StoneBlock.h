#ifndef STONEBLOCK_H
#define STONEBLOCK_H

#include "Block.h"

class StoneBlock : public Block {
public:
    StoneBlock(Vector2 pos, Vector2 size, Color color);
    StoneBlock(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~StoneBlock();

    void draw() override;
    void update() override;

};

#endif // STONEBLOCK_H