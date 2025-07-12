#ifndef PIRANHAPLANT_H
#define PIRANHAPLANT_H

#include "Enemy.h"
class PiranhaPlant : public Enemy {
public:
    PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void draw();
    void update(Mario& mario, const std::vector<Sprite*>& collidables) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Mario& mario);
private:
    PiranhaState piranhaState;

    float stateTimer;
    float stateDuration;       // thời gian chờ mỗi trạng thái (VD: 1.5f)
    float riseSpeed;           // tốc độ leo lên (VD: 30.0f)

    float hiddenY;             // vị trí ẩn hoàn toàn
    float shownY;

};

enum class PiranhaState {
    HIDING,            // ẩn/đóng miệng
    RISING,            // đi lên/ mở miệng
    SHOWN,             // lộ hoàn toàn/ mở miệng
    LOWERING,          // đi xuống/đóng miệng
    DYING              // đứng yên/nổ
};

#endif
