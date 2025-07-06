#ifndef BUZZYBETTLE_H
#define BUZZYBETTLE_H
#include "Enemy/Enemy.h"

class BuzzyBettle : public Enemy {
public:
    BuzzyBettle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update() override;
    void draw() override;
    void beingHit() override;
    void collisionSound() override;

    void kickShell(bool faceLeft); // gọi từ Mario khi đá mai
    bool isShellMoving() const;    // để Mario kiểm tra

private:
    bool shellMoving;
    float shellSpeed;
    bool isFacingLeft;
};

#endif