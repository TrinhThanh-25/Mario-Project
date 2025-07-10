#ifndef THREEUPMOON_H
#define THREEUPMOON_H

#include "Item.h"

class ThreeUpMoon : public Item {
private:
    int lives;
public:
    ThreeUpMoon(Vector2 position, Vector2 size, Color color, int lives);
    ~ThreeUpMoon() override;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 