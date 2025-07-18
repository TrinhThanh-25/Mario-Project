#ifndef CHOOSECHARACTERSTATE_H
#define CHOOSECHARACTERSTATE_H 

#include "GameState/GameState.h"
#include "Game/World.h"
#include "GUI/Button.h"
#include "Character/CharacterTag.h"

class ChooseCharacterState : public GameState {
    private:
        ModeWorld modeWorld;
        CharacterTag singlePlayerTag;
        CharacterTag firstPlayerTag;
        CharacterTag secondPlayerTag;
        Button onePlayerButton;
        Button twoPlayersButton;
    public:
        ChooseCharacterState(World* world);
        ~ChooseCharacterState() override;
        void update() override;
        void draw() override;
};

#endif