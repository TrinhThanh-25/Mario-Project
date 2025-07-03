#ifndef ENEMY_H
#define ENEMY_H

#include "Common/Sprite.h"
//#include "Mario/Mario.h"
#include "raylib.h"

class Enemy : public Sprite {
public:

    Enemy(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    virtual ~Enemy();

    virtual void update();
    virtual void draw();

    virtual void activeWhenMarioApproach(Mario& mario);
    virtual void beingHit();
    virtual void followTheLeader(Sprite* leader);
    virtual void collisionSound();

    Rectangle getCollisionBox();
    // CollisionType checkCollision(Block* block);
    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

protected:
    float dyingFrameAcum;
    float dyingFrameTime;
    int maxDyingFrame;
    int currentDyingFrame;
    float pointFrameAcum;
    float pointFrameTime;
    Vector2 diePosition;
};

#endif