#ifndef MUSHROOM_H
#define MUSHROOM_H

#include "Item.h"

class Mushroom : public Item {
private:
    int points;

    // Atributes for Inventory
    bool isInInventory;
    float blinkFrameTime = 0.1f; 
    float blinkFrameAccum = 0.0f;
    bool appear = false;

public:
    Mushroom(Vector2 position, Vector2 size, Color color, int points, bool isInInventory);
    ~Mushroom() override = default;

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