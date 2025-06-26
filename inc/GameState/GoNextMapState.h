#ifndef GONEXTMAPSTATE_H
#define GONEXTMAPSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class GoNextMapState : public GameState {
    private:
    public:
        GoNextMapState(World* world);
        ~GoNextMapState() override;
        void update() override;
        void draw() override;
};

#endif