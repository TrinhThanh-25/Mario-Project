#ifndef GOOMBA_H
#define GOOMBA_H

#include "Enemy.h"
#include "Game/World.h"

class Goomba : public Enemy{
public:
    Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~Goomba();

    void draw();
    void update(const std::vector<Character*>& characterList) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Character& character);

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);
};


#endif