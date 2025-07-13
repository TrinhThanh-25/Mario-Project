#ifndef COLLISIONTYPE_H
#define COLLISIONTYPE_H

enum class CollisionType {
    NONE = 0,
    COLLIDED,

    NORTH,
    SOUTH,
    WEST,
    EAST,
    
    FIREBALL,
};

#endif