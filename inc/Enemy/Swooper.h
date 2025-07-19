#ifndef SWOOPER_H
#define SWOOPER_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class Swooper : public Enemy {
public:
    Swooper(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override;

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

private:
    Vector2 startPosition;          // Vị trí bắt đầu rơi
    bool isDropping;            // Bắt đầu rơi xuống chưa
    bool isFlyingHorizontally;  // Bay ngang sau khi rơi xuống
    float dropSpeed;            // Tốc độ rơi
    float flySpeed;             // Tốc độ bay ngang
    float activationRangeY;     // Khoảng cách theo chiều dọc để kích hoạt
};

#endif
