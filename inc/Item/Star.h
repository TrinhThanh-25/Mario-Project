#ifndef STAR_H
#define STAR_H

#include "Item.h"

class Star : public Item {
private:
    int points;
public:
    Star(Vector2 position, Vector2 size, Color color, int points);
    ~Star() override;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif
