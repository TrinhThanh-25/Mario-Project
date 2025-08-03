#ifndef TESTMAPSTATE_H
#define TESTMAPSTATE_H

#include "GameState.h"

class TestMapState : public GameState {
    public:
        TestMapState(World* world);
        ~TestMapState() override;
        void update() override;
        void draw() override;
};

#endif