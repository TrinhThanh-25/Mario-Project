#ifndef JUMPING_PIRANHA_PLANT_H
#define JUMPING_PIRANHA_PLANT_H

#include "Enemy/Enemy.h"
#include "Game/World.h"

enum class JumpingPiranhaState {
    IDLE,       // Đứng chờ dưới ống
    JUMPING,    // Đang nhảy lên
    FALLING     // Đang rơi xuống
};

class JumpingPiranhaPlant : public Enemy {
public:
    JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterList) override;
    void draw() override;
    void beingHit(HitType type) override;
    void activeWhenMarioApproach(Character& character) override;
    void collisionBlock(Block* block) override;
    void collisionTile(Tile* tile) override;

    json saveToJson() const;
    void loadFromJson(const json& j);

private:
    JumpingPiranhaState jumpState;

    float jumpSpeed;         // Tốc độ nhảy lên
    float gravity;           // Gia tốc trọng lực
    float waitDuration;      // Thời gian chờ giữa các cú nhảy
    float waitTimer;         // Thời gian đếm để bắt đầu nhảy

    float groundY;           // Vị trí y ban đầu (mặt ống)
    float maxJumpHeight;
};

#endif
