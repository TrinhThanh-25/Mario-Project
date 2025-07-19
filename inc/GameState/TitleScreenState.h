#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "GUI/Button.h"

class TitleScreenState : public GameState {
    private:
        Button newGameButton;
        Button continueButton;
        Button optionsButton;
        Button exitButton;
        bool isSavedGameAvailable;
    public:
        TitleScreenState(World* world);
        ~TitleScreenState() override;

        void update() override;
        void draw() override;
};

#endif 