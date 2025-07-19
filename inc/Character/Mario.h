#ifndef MARIO_H
#define MARIO_H

#include "Character/Character.h"

class Mario : public Character {
    public:
        Mario(ModePlayer mode,Vector2 pos, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        ~Mario();
        void updateCollisionBoxes() override;
        void transitionToSmall() override;
        void transitionToSuper() override;
        void transitionToFlower() override;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif