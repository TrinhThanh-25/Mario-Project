#ifndef LUIGI_H
#define LUIGI_H

#include "Character/Character.h"

class Luigi : public Character {
    public:
        Luigi(ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        ~Luigi();
};

#endif