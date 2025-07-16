#ifndef FLYINGGOOMBA_H
#define FLYINGGOOMBA_H

#include "Enemy.h"

class FlyingGoomba : public Enemy {
public:
    FlyingGoomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~FlyingGoomba();

    void draw();
    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Mario& mario);
private:
    float jumpTimer = 0.0f;
    float jumpInterval = 1.5f; // mỗi 1.5s nhảy 1 lần
    float jumpSpeed = -150.0f; // tốc độ nhảy lên
};

#endif