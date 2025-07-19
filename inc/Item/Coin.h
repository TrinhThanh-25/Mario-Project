#ifndef COIN_H
#define COIN_H

#include "Item.h"

class Coin : public Item {
private:
    int points;
public:
    Coin(Vector2 position, Vector2 size, Color color, int points);
    ~Coin() override = default;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;

    json saveToJson() const override;
    void loadFromJson(const json& j) override;
};

#endif 
