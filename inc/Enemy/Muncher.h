#ifndef MUNCHER_H
#define MUNCHER_H

#include "Enemy/Enemy.h"
#include "Game/World.h"

class Muncher : public Enemy {
public:
    Muncher(Vector2 pos, Vector2 dim, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override; // không làm gì
    void activeWhenMarioApproach(Character& character) override; 

    void collisionTile(Tile* tile) override;
    void collisionBlock(Block* block) override; // không làm gì
};

#endif
