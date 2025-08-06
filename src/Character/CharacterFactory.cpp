#include "Character/CharacterFactory.h"
#include "Character/Mario.h"
#include "Character/Luigi.h"
#include "Character/Toad.h"
#include "Character/Peach.h"

Character* CharacterFactory::createCharacter(CharacterName characterName, ModePlayer mode) {
    float defaultSpeedX = 260.0f;
    float defaultMaxSpeedX = 360.0f;
    float defaultJumpSpeed = -600.0f;
    Vector2 defaultPosition = {64, 0};
    Vector2 defaultVelocity = {0, 0};
    int initialLives = 5;
    switch (characterName) {
        case CharacterName::MARIO:
            return new Mario(mode, defaultPosition, defaultVelocity, RED, defaultSpeedX, defaultMaxSpeedX, defaultJumpSpeed, initialLives);
        case CharacterName::LUIGI:
            return new Luigi(mode, defaultPosition, defaultVelocity, GREEN, defaultSpeedX, defaultMaxSpeedX, defaultJumpSpeed, initialLives);
        case CharacterName::TOAD:
            return new Toad(mode, defaultPosition, defaultVelocity, BLUE, defaultSpeedX, defaultMaxSpeedX, defaultJumpSpeed, initialLives);
        case CharacterName::PEACH:
            return new Peach(mode, defaultPosition, defaultVelocity, PINK, defaultSpeedX, defaultMaxSpeedX, defaultJumpSpeed, initialLives);
        default:
            return nullptr; 
    }
}

