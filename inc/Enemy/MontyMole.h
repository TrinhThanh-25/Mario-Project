#ifndef MONTYMOLE_H
#define MONTYMOLE_H

#include "Enemy/Enemy.h"
#include "Common/HitType.h"

class MontyMole : public Enemy{
public:
    MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color);
    ~MontyMole();

    void draw();
    void update(const std::vector<Character*>& characterList) override;
    void beingHit(HitType type);
    void collisionSound();
    void activeWhenMarioApproach(Character& character);

    void collisionBlock(Block* block);
    void collisionTile(Tile* tile);

private:
    float emergeDelay;   // thời gian chờ để chui lên
    float emergeTimer;    // đếm thời gian thực tế để so với Delay
    bool hasEmerge;      // xem trồi lên hay chưa
};

#endif