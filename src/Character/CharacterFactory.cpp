#include "Character/CharacterFactory.h"
#include "Character/Mario.h"
#include "Character/Luigi.h"
#include "Character/Toad.h"
#include "Character/Peach.h"

Character* CharacterFactory::createCharacter(CharacterName characterName, ModePlayer mode) {
    float defaultSpeedX;
    float defaultMaxSpeedX;
    float defaultJumpSpeed;
    Vector2 defaultPosition;
    Vector2 defaultVelocity;
    float acceleration;
    float friction;
    float floatTime;
    int initialLives = 5;
    switch (characterName) {
        case CharacterName::MARIO:
            defaultSpeedX = 260.0f;
            defaultMaxSpeedX = 360.0f;
            defaultJumpSpeed = -630.0f;
            defaultPosition = {100, 100};
            defaultVelocity = {0, 0};
            acceleration = 1500.0f;
            friction = 1200.0f;
            floatTime = 0.0f;
            break;
        case CharacterName::LUIGI:
            defaultSpeedX = 234.0f;
            defaultMaxSpeedX = 324.0f;
            defaultJumpSpeed = -750.0f;
            defaultPosition = {100, 100};
            defaultVelocity = {0, 0};
            acceleration = 1200.0f;
            friction = 600.0f;
            floatTime = 0.0f;
            break;
        case CharacterName::TOAD:
            defaultSpeedX = 312.0f;
            defaultMaxSpeedX = 432.0f;
            defaultJumpSpeed = -560.0f;
            defaultPosition = {100, 100};
            defaultVelocity = {0, 0};
            acceleration = 1950.0f;
            friction = 1440.0f;
            floatTime = 0.0f;
            break;
        case CharacterName::PEACH:
            defaultSpeedX = 220.0f;
            defaultMaxSpeedX = 305.0f;
            defaultJumpSpeed = -580.0f;
            defaultPosition = {100, 100};
            defaultVelocity = {0, 0};
            acceleration = 1275.0f;
            friction = 1320.0f;
            floatTime = 1.5f;
            break;
        default:
            return nullptr;
    }
    switch (characterName) {
        case CharacterName::MARIO:
            return new Mario(mode, defaultPosition, defaultVelocity, RED, defaultSpeedX, defaultMaxSpeedX, acceleration, friction, floatTime, defaultJumpSpeed, initialLives);
        case CharacterName::LUIGI:
            return new Luigi(mode, defaultPosition, defaultVelocity, GREEN, defaultSpeedX, defaultMaxSpeedX, acceleration, friction, floatTime, defaultJumpSpeed, initialLives);
        case CharacterName::TOAD:
            return new Toad(mode, defaultPosition, defaultVelocity, BLUE, defaultSpeedX, defaultMaxSpeedX, acceleration, friction, floatTime, defaultJumpSpeed, initialLives);
        case CharacterName::PEACH:
            return new Peach(mode, defaultPosition, defaultVelocity, PINK, defaultSpeedX, defaultMaxSpeedX, acceleration, friction, floatTime, defaultJumpSpeed, initialLives);
        default:
            return nullptr; 
    }
}

