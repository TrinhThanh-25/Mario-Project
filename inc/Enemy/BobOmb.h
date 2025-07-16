#ifndef BOBOMB_H
#define BOBOMB_H

#include "Enemy/Enemy.h"

class BobOmb : public Enemy {
public:
    BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Mario& mario) override;
private:
    bool isIgnited;           // Đã bị kích hoạt chưa
    float ignitionTimer;      // Thời gian đếm ngược trước khi nổ
    float maxIgniteTime;      // VD: 2.0s
    float explosionRadius;    // vùng gây sát thương

};

#endif