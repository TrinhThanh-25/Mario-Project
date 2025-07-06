#ifndef FINISHEDSTATE_H
#define FINISHEDSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class FinishedState : public GameState {
    private:
    public:
        FinishedState(World* world);
        ~FinishedState() override;
        void update() override;
        void draw() override;
};

#endif 