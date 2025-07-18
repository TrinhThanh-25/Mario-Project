#ifndef PIRANHAPLANT_H
#define PIRANHAPLANT_H

#include "Enemy.h"

enum class PiranhaState {
    HIDING,            // ẩn/đóng miệng
    RISING,            // đi lên/ mở miệng
    SHOWN,             // lộ hoàn toàn/ mở miệng
    LOWERING,          // đi xuống/đóng miệng
    DYING              // đứng yên/nổ
};

class PiranhaPlant : public Enemy {
public:
    PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void draw();
    void update(const std::vector<Character*>& characterList) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Character& character);
    void collisionBlock(Block* block) override;
    void collisionTile(Tile* tile) override;
private:
    PiranhaState piranhaState;

    float stateTimer;
    float stateDuration;       // thời gian chờ mỗi trạng thái (VD: 1.5f)
    float riseSpeed;           // tốc độ leo lên (VD: 30.0f)

    float hiddenY;             // vị trí ẩn hoàn toàn
    float shownY;

};

#endif
