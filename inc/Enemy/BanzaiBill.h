#ifndef BANZAI_BILL_H
#define BANZAI_BILL_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class BanzaiBill : public Enemy {
public:
    BanzaiBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override;
    void collisionBlock(Block* block) override;
    void collisionTile(Tile* tile) override;

    json saveToJson() const;
    void loadFromJson(const json& j);

private:
    float maxLifeTime;    // Tự biến mất sau 1 khoảng thời gian
    float lifeTimer;
};

#endif
