#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Item.h"

class Mushroom : public Item {
private:
    int points;

    // Atributes for Inventory
    bool isInInventory = false;
    float blinkFrameTime = 0.1f; 
    float blinkFrameAccum;
    bool appear;

public:
    Mushroom(Vector2 position, Vector2 size, Color color, int points, bool isInInventory);
    ~Mushroom() override;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 