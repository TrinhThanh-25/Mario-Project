#ifndef CHOOSECHARACTERSTATE_H
#define CHOOSECHARACTERSTATE_H 

#include "GameState/GameState.h"
#include "GUI/CharacterTag.h"
#include "Game/ModeWorld.h"

class ChooseCharacterState : public GameState {
    private:
        ModeWorld modeWorld;
        std::vector<CharacterTag*> characterTags;
        int curP1 = 0;
        int curP2 = 1;
    public:
        ChooseCharacterState(World* world);
        ~ChooseCharacterState() override;
        void update() override;
        void draw() override;
        void setModeWorld(ModeWorld mode);
        void enter() override;
        void exit() override;
};

#endif