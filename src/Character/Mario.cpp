#include "Character/Mario.h"

Mario::Mario(ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed) :
    Character("Mario",mode , pos, dim, vel, color, speedX, maxSpeedX, jumpSpeed) {
}

Mario::~Mario() {
    
}