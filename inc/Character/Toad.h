#ifndef MARIO_CHARACTER_TOAD_H
#define MARIO_CHARACTER_TOAD_H

#include "Character/Character.h"

class Toad : public Character {
    public:
        Toad(ModePlayer mode, Vector2 pos, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, int initialLives);
        ~Toad() override;

        void updateCollisionBoxes() override;
        void transitionToSmall() override;
        void transitionToSuper() override;
        void transitionToFlower() override;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif