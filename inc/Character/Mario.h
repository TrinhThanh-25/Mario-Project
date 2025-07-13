#ifndef MARIO_H
#define MARIO_H

#include "Character/Character.h"

class Mario : public Character {
    public:
        Mario(ModePlayer mode,Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        ~Mario();
};

#endif