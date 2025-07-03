#ifndef SPRITESTATE_H
#define SPRITESTATE_H

enum SpriteState {
    //All objects
    IDLE,
    ACTIVE,
    TO_BE_REMOVED,
    
    HIT,
    DYING,
    NEITHER,

    //Only Mario
    ON_GROUND,
    JUMPING,
    FALLING,
    SMALL_TO_SUPER,
    SMALL_TO_FLOWER,
    SUPER_TO_FLOWER,
    SUPER_TO_SMALL,
    FLOWER_TO_SMALL,
    VICTORY,
    TO_NEXT_MAP,
    INVULNERABLE,
	
    //Other objects
    INVISIBLE,
};

#endif