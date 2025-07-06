#ifndef ENEMY_H
#define ENEMY_H

#include "Common/Sprite.h"
//#include "Mario/Mario.h"
#include "raylib.h"
#include "Character/Mario.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Common/HitType.h"


class Enemy : public Sprite {
public:

    Enemy(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    virtual ~Enemy();

    virtual void update();
    virtual void draw() = 0;

    virtual void activeWhenMarioApproach(Mario& mario);
    virtual void beingHit(HitTyple type);
    virtual void followTheLeader(Sprite* leader);
    virtual void collisionSound() = 0;

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

    bool isFacingLeft;
};

#endif