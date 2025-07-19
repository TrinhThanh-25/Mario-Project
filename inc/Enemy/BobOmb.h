#ifndef BOBOMB_H
#define BOBOMB_H

#include "Enemy/Enemy.h"

enum class BobOmbState {
    IDLE,       // Bình thường
    IGNITED,    // Đã kích hoạt, đang đếm giờ
    EXPLODED    // Đã nổ
};

class BobOmb : public Enemy {
public:
    BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override;

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

    json saveToJson() const;
    void loadFromJson(const json& j);
private:
    bool isIgnited;           // Đã bị kích hoạt chưa
    float ignitionTimer;      // Thời gian đếm ngược trước khi nổ
    float maxIgniteTime;      // VD: 2.0s
    float explosionRadius;    // vùng gây sát thương
    BobOmbState bobombState;

};


#endif