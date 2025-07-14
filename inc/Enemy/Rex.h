#ifndef REX_H
#define REX_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class Rex : public Enemy {
public:
    Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;

    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override;

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

private:
    bool isShrunken;          // Đã bị dẫm 1 lần chưa
    float shrinkDuration;     // Thời gian để giữ trạng thái thu nhỏ (có thể không cần nếu muốn giữ vĩnh viễn)
};

#endif
