#ifndef ENEMY_H
#define ENEMY_H

#include "Common/Sprite.h"
//#include "Mario/Mario.h"
#include "raylib.h"
#include "Character/Mario.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Common/HitType.h"

enum class EnemyType {
    BANZAI_BILL,
    BLUE_KOOPA_TROOPA,
    BOB_OMB,
    BULLET_BILL,
    BUZZY_BEETLE,
    FLYING_GOOMBA,
    GOOMBA,
    GREEN_KOOPA_TROOPA,
    JUMPING_PIRANHA_PLANT,
    MONTY_MOLE,
    MUMMY_BEETLE,
    MUNCHER,
    PIRANHA_PLANT,
    RED_KOOPA_TROOPA,
    REX,
    SWOOPER,
    YELLOW_KOOPA_TROOPA
};

class Enemy : public Sprite {
public:

    Enemy(EnemyType enemyType, Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    virtual ~Enemy();

    virtual void update(const std::vector<Character*>& characterList) = 0;
    virtual void update() override;
    virtual void draw() = 0;

    virtual void activeWhenMarioApproach(Character& character);
    virtual void beingHit(HitType type);
    virtual void followTheLeader(Sprite* leader);
    virtual void collisionSound();

    Rectangle getCollisionBox();
    // CollisionType checkCollision(Block* block);
    virtual void collisionBlock(Block* block);
    virtual void collisionTile(Tile* tile);

    MoveType movetype;
    void setMoveType(MoveType type) { movetype = type; }
    MoveType getMoveType() const { return movetype; }
    EnemyType getEnemyType();

    // CollisionType checkCollision(const std::vector<Sprite*>& collidables);


protected:
    float dyingFrameAcum;
    float dyingFrameTime = 0.2f;
    int maxDyingFrame;
    int currentDyingFrame;
    float pointFrameAcum;
    float pointFrameTime;
    Vector2 diePosition;

    bool isFacingLeft;
    EnemyType type;
};

#endif