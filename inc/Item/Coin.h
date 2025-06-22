#ifndef COIN_H
#define COIN_H

#include "Item.h"

class Coin : public Item {
private:
public:
    Coin();
    Coin(Vector2 position, Vector2 size, Vector2 velocity, Color color, float frameTime, int maxFrames, Direction facingDirection);
    ~Coin() override;

    void update() override;
    void draw() override;

    void playCollisionSound() override;
    void updateMario(Mario& mario) override;
    void collisionSouth(Mario& mario) override;
};

#endif 
