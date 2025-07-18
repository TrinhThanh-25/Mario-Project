#ifndef REDKOOPATROOPA_H
#define REDKOOPATROOPA_H

#include "Enemy/Enemy.h"

class RedKoopaTroopa : public Enemy {
public:
    RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~RedKoopaTroopa();

    void draw();
    void update(const std::vector<Character*>& characterList) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Character& character);

    void followTheLeader(Sprite* leader);

    Sprite* leader = nullptr;

    bool isNearEdge();

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);
private:
    bool shellMoving = false;
    float shellSpeed = 250.0f;

};


#endif