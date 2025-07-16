#ifndef GREENKOOPATROOPA_H
#define GREENKOOPATROOPA_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class GreenKoopaTroopa : public Enemy {
public:
    GreenKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void draw() override;

    void beingHit(HitType type) override;
    void kickShell(bool faceLeft);
    bool isShellMoving() const;

    void activeWhenMarioApproach(Mario& mario) override;

    void followTheLeader(Sprite* leader);

    Sprite* leader = nullptr;


private:
    bool shellMoving;        // Shell đang lăn không
    float shellTimer;        // Thời gian trong shell
    float shellSpeed;        // Tốc độ lăn shell
    float extraWakeUpTime;   // Delay thêm sau thời gian cố định (nếu có)

    // Có thể thêm biến animation frame nếu cần
};

#endif
