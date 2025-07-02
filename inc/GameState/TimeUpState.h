#ifndef TIMEUPSTATE_H
#define TIMEUPSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class TimeUpState : public GameState {
    private:
    public:
        TimeUpState(World* world);
        ~TimeUpState() override;

        void update() override;
        void draw() override;
};

#endif