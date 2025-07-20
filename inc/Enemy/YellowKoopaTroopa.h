#ifndef YELLOWKOOPATROOPA_H
#define YELLOWKOOPATROOPA_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"
#include "Game/World.h"

class YellowKoopaTroopa : public Enemy {
public:
    YellowKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;

    void beingHit(HitType type) override;
    void kickShell(bool faceLeft);
    bool isShellMoving() const;

    void activeWhenMarioApproach(Character& character) override;

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

    json saveToJson() const;
    void loadFromJson(const json& j);
private:
    bool shellMoving;        // Shell đang lăn không
    float shellTimer;        // Thời gian trong shell
    float shellSpeed;        // Tốc độ lăn shell
    float extraWakeUpTime;   // Delay thêm sau thời gian cố định (nếu có)

    // Có thể thêm biến animation frame nếu cần
};

#endif
