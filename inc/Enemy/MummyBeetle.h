#ifndef MUMMYBETTLE_H
#define MUMMYBETTLE_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"
#include "Game/World.h"

class MummyBeetle : public Enemy {
public:
    MummyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color);

    void update(const std::vector<Character*>& characterListr) override;
    void draw() override;

    void beingHit(HitType type) override;
    void kickShell(bool faceLeft);
    bool isShellMoving() const;

    void activeWhenMarioApproach(Character& character);

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

    json saveToJson() const;
    void loadFromJson(const json& j);

private:
    bool shellMoving;
    float shellTimer;
    float shellSpeed;
    float extraWakeUpTime;

    
};


#endif