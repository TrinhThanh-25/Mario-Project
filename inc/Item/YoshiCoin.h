#ifndef YOSHICOIN_H
#define YOSHICOIN_H

#include "Item.h"

class YoshiCoin : public Item {
private:
    int points;
public:
    YoshiCoin(Vector2 position, Vector2 size, Color color, int points);
    ~YoshiCoin() override = default;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 