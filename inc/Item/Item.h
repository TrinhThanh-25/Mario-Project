#ifndef ITEM_H
#define ITEM_H

#include "Common/Sprite.h"
#include "Character/Character.h"
#include "raylib.h"
#include "Block/Block.h"
#include "Tile/Tile.h"
#include "Common/ResourceManager.h"

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
        virtual void updateWhenActive(float timeElapsed) = 0;
        virtual void updateWhenHit(float timeElapsed) = 0;
    void draw() override;

    virtual void playCollisionSound() = 0;
    virtual void updateCharacter(Character* character) = 0;
    virtual void collisionSouth(Character* character) = 0;
    bool isPausedGameWhenBeingHit();

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);
    void collisionCharacter(Character* character);
};

#endif