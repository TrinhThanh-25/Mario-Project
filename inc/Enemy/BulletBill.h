#ifndef BULLETBILL_H
#define BULLETBILL_H

#include "Enemy/Enemy.h"

class BulletBill : public Enemy {
public:
    BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Mario& mario) override;
};

#endif
