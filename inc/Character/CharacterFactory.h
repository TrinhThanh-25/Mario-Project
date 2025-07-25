#ifndef CHARACTERFACTORY_H
#define CHARACTERFACTORY_H

#include "Character/Character.h"

class CharacterFactory {
    public:
        static Character* createCharacter(CharacterName characterName, ModePlayer mode);
};

#endif