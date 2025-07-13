#ifndef FIREFLOWER_H
#define FIREFLOWER_H

#include "Item.h"

class FireFlower : public Item {
private:
    int points;
    
    // Atributes for Inventory
    bool isInInventory;
    float blinkFrameTime = 0.1f; 
    float blinkFrameAccum = 0.0f;
    bool appear = false;
public:
    FireFlower(Vector2 position, Vector2 size, Color color, int points,bool isInInventory);
    ~FireFlower() override = default;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 