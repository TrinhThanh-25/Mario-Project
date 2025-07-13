#ifndef GOOMBA_H
#define GOOMBA_H

#include "Enemy.h"

class Goomba : public Enemy{
public:
    Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~Goomba();

    void draw();
    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Mario& mario);
};


#endif