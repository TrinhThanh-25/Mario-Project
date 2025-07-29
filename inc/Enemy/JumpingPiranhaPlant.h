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
// jumping logic
    float groundY;
    float minY;
    float jumpSpeed = 200.0f;

    bool goingUp = true;
    bool waiting = false;
    float waitDuration = 2.0f;
    float waitTimer = 0.0f;


    
};

#endif
