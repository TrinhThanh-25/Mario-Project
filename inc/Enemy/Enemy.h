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

    virtual void update(Mario& mario, const std::vector<Sprite*>& collidables) = 0;

    virtual void draw() = 0;

    virtual void activeWhenMarioApproach(Mario& mario);
    virtual void beingHit(HitType type);
    virtual void followTheLeader(Sprite* leader);
    virtual void collisionSound() = 0;

    Rectangle getCollisionBox();
    // CollisionType checkCollision(Block* block);
    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

    MoveType movetype;
    void setMoveType(MoveType type) { movetype = type; }
    MoveType getMoveType() const { return movetype; }

    CollisionType checkCollision(const std::vector<Sprite*>& collidables);


protected:
    float dyingFrameAcum;
    float dyingFrameTime = 0.2f;
    int maxDyingFrame;
    int currentDyingFrame;
    float pointFrameAcum;
    float pointFrameTime;
    Vector2 diePosition;

    bool isFacingLeft;
};

#endif