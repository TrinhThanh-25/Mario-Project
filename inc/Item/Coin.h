#ifndef COIN_H
#define COIN_H

#include "Item.h"

class Coin : public Item {
private:
    int points;
public:
    Coin();
    Coin(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction,float HitFrameTime, int maxHitFrame, bool pause,int points);
    ~Coin() override;

    void update() override;
    void draw() override;

    void playCollisionSound() override;
    void updateMario(Mario& mario) override;
    void collisionSouth(Mario& mario) override;
};

#endif 
