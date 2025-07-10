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
    INACTIVE,
    TO_BE_REMOVED,

    HIT,
    NEITHER,


    // Auxiliary State
    INVULNERABLE,
    INVISIBLE,

    //
    NO_COLLIDABLE,

    // New enemy state
    SHELL,
    SHELL_MOVING,
    HIDDEN,

    PRIMED,  // đã kích nỗ
    IGNITED, // đếm ngược
    EXPLODING, // đang nổ
    

};

enum MoveType{
    FLYING,
    WALKING,
}

#endif