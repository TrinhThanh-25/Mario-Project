#include "Character/Luigi.h"

Luigi::Luigi(ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed) :
    Character("Luigi", mode, pos, dim, vel, color, speedX, maxSpeedX, jumpSpeed) {
}

Luigi::~Luigi() {
    
}