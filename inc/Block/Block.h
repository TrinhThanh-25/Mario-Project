#ifndef BLOCK_H
#define BLOCK_H

#include "Common/Sprite.h"
#include "Mario/Mario.h"
#include "raylib.h"

class Block : public Sprite{
public:
    Block(Vector2 pos, Vector2 size, Color color);
    Block(Vector2 pos, Vector2 size, Color color, float frameTime, int maxFrames);
    ~Block() override;

    void update() override = 0;
    void draw() override = 0;
};

#endif