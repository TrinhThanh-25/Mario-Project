#ifndef SPRITESTATE_H
#define SPRITESTATE_H

enum SpriteState {
    //All objects
    DYING,

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

    //Other objects
    IDLE,
    ACTIVE,
    TO_BE_REMOVED,

    HIT,
    NEITHER,


    // Auxiliary State
    INVULNERABLE,
    INVISIBLE,

    //
    NO_COLLIDABLE,
};

#endif