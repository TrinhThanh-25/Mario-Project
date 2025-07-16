#ifndef COURSECLEARTOKEN_H
#define COURSECLEARTOKEN_H

#include "Item.h"

class CourseClearToken : public Item {
private:
    int minY;
    int maxY;
    int points;
public:
    CourseClearToken(Vector2 position, Vector2 size, Color color);
    ~CourseClearToken() override = default;

    void update() override;
        void updateWhenActive(float timeElapsed) override;
        void updateWhenHit(float timeElapsed) override;
    void draw() override;

    void playCollisionSound() override;
    void updateCharacter(Character* character) override;
    void collisionSouth(Character* character) override;
};

#endif 