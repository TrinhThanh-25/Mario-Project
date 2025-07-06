#ifndef ITEM_H
#define ITEM_H

#include "Common/Sprite.h"
#include "Mario/Mario.h"
#include "raylib.h"
#include "Block/Block.h"
#include "Tile/Tile.h"

class Item : public Sprite{
protected:
    float beingHitFrameTime;
    float beingHitFrameAcum;
    int maxBeingHitFrame;
    int currentBeingHitFrame;
    float pointFrameAccum;
    float pointFrameTime;
    bool pauseGameWhenHit;
public:
    Item();
    Item(Vector2 position, Vector2 size, Color color);
    Item(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction,float HitFrameTime, int maxHitFrame, bool pause);
    ~Item() override;
    
    void update() override;
    void draw() override;

    virtual void playCollisionSound() = 0;
    virtual void updateMario(Mario& Mario) = 0;
    virtual void collisionSouth(Mario& Mario) = 0;
    virtual bool isPausedGameWhenBeingHit();

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);
    void collisionCharacter(Mario* mario);
};

#endif