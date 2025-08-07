#ifndef PIRANHAPLANT_H
#define PIRANHAPLANT_H

#include "Enemy.h"
#include "Game/World.h"

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

    json saveToJson() const;
    void loadFromJson(const json& j);
private:
    float minY;
    float maxY;
    float velocityUp;
    float waitTime;
    float waitAcum;
    bool waiting;
};

#endif
