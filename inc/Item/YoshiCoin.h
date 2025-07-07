#ifndef YOSHICOIN_H
#define YOSHICOIN_H

#include "Item.h"

class Yoshicoin : public Item {
private:
    int points;
public:
    Yoshicoin(Vector2 position, Vector2 size, Color color, int points);
    ~Yoshicoin() override;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 