#ifndef BULLETBILL_H
#define BULLETBILL_H

#include "Enemy/Enemy.h"

class BulletBill : public Enemy {
public:
    BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override; // Không cần kích hoạt

    void collisionTile(Tile* tile) override;
    void collisionBlock(Block* block) override;
};

#endif
