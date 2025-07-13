#ifndef MUNCHER_H
#define MUNCHER_H

#include "Enemy/Enemy.h"

class Muncher : public Enemy {
public:
    Muncher(Vector2 pos, Vector2 dim, Color color);

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;
    void beingHit(HitType type) override; // không làm gì
    void activeWhenMarioApproach(Mario& mario) override; // luôn luôn active
};

#endif
