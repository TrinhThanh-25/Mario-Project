#ifndef COUNTINGPOINTSTATE_H
#define COUNTINGPOINTSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class CountingPointState : public GameState {
    private:
        int* remainTimePoint;
        GameHud* gameHud;
        Map* map;
        Camera2D* camera;
    public:
        CountingPointState(World* world);
        ~CountingPointState() override;
        void update() override;
        void draw() override;
};

#endif