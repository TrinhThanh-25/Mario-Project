#ifndef PEACH_H
#define PEACH_H

#include "Character/Character.h"

class Peach : public Character {
    public:
        Peach(ModePlayer mode, Vector2 pos, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        ~Peach();

        void updateCollisionBoxes() override;
        void transitionToSmall() override;
        void transitionToSuper() override;
        void transitionToFlower() override;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif