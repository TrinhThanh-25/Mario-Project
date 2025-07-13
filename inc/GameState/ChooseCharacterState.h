#ifndef CHOOSECHARACTERSTATE_H
#define CHOOSECHARACTERSTATE_H 

#include "GameState/GameState.h"
#include "Game/World.h"

class ChooseCharacterState : public GameState {
    private:
    public:
        ChooseCharacterState(World* world);
        ~ChooseCharacterState() override;
        void update() override;
        void draw() override;
};

#endif