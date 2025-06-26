#ifndef IRISOUTSTATE_H
#define IRISOUTSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class IrisOutState : public GameState {
    private:
    public:
        IrisOutState(World* world);
        ~IrisOutState() override;
        void update() override;
        void draw() override;
};

#endif