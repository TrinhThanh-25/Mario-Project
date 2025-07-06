#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class SettingState : public GameState {
    private:
    public:
        SettingState(World* world);
        ~SettingState() override;

        void update() override;
        void draw() override;
};

#endif