#ifndef ONEUPMUSHROOM_H
#define ONEUPMUSHROOM_H

#include "Item.h"

class OneUpMushroom : public Item {
public:
    OneUpMushroom(Vector2 position, Vector2 size, Color color, int lives);
    ~OneUpMushroom() override = default;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;
    void playCollisionSound() override;
    void updateCharacter(Character *character) override;
    void collisionSouth(Character *character) override;
    json saveToJson() const override;
    void loadFromJson(const json &j) override;

private:
    int lives;
};

#endif // ONEUPMUSHROOM_H
