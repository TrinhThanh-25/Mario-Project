#ifndef GONEXTMAPSTATE_H
#define GONEXTMAPSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include <vector>

class GoNextMapState : public GameState {
    private:
        float irisOutTime;
        float irisOutAcum;
        bool isIrisOutFinished;
        Map* map;
        std::vector<Character*>& characters;
        GameHud* gameHud;
        Camera2D* camera;
    public:
        GoNextMapState(World* world);
        ~GoNextMapState() override;
        void update() override;
        void draw() override;
};

#endif