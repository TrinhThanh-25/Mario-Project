#ifndef TITLESCREENSTATE_H
#define TITLESCREENSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class TitleScreenState : public GameState {
    private:
    public:
        TitleScreenState(World* world);
        ~TitleScreenState() override;

        void update() override;
        void draw() override;
};

#endif 