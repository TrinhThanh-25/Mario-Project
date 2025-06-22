#ifndef ITEM_H
#define ITEM_H

#include "Common/Sprite.h"
#include "Mario/Mario.h"
#include "raylib.h"

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
    ~Item() override;
    void update() override;
    void draw() override;

    virtual void playCollisionSound() = 0;
    virtual void updateMario(Mario& Mario) = 0;
    virtual void collisionSouth(Mario& Mario) = 0;
    virtual bool isPausedGameWhenBeingHit();

};

#endif