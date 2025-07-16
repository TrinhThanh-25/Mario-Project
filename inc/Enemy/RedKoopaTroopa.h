#ifndef REDKOOPATROOPA_H
#define REDKOOPATROOPA_H

#include "Enemy/Enemy.h"

class RedKoopaTroopa : public Enemy {
public:
    RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~RedKoopaTroopa();

    void draw();
    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Mario& mario);

    void followTheLeader(Sprite* leader);

    Sprite* leader = nullptr;

    bool isNearEdge();
private:
    bool shellMoving = false;
    float shellSpeed = 250.0f;

};


#endif