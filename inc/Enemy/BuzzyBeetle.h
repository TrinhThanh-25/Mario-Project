#ifndef BUZZYBETTLE_H
#define BUZZYBETTLE_H
#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class BuzzyBeetle : public Enemy {
public:
    BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~BuzzyBeetle();

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;

    void beingHit(HitType type) override;
    void collisionSound() override;

    void kickShell(bool faceLeft); // gọi từ Mario khi đá mai
    bool isShellMoving() const;    // để Mario kiểm tra

    void activeWhenMarioApproach(Mario& mario);

private:
    bool shellMoving;
    float shellSpeed;
    float shellTimer;
};

#endif