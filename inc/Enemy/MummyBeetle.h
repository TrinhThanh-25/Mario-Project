#ifndef MUMMYBETTLE_H
#define MUMMYBETTLE_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class MummyBeetle : public Enemy {
public:
    MummyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;

    void beingHit(HitType type) override;
    void kickShell(bool faceLeft);
    bool isShellMoving() const;

    void activeWhenMarioApproach(Mario& mario);

private:
    bool shellMoving;
    float shellTimer;
    float shellSpeed;
    float extraWakeUpTime;

    
};


#endif