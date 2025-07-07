#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "Common/Button.h"

class SettingState : public GameState {
    private:
        Button resumeButton;
        Button restartButton;
        Button returnButton;
        Button exitButton;

        bool isTitleBefore;
    public:
        SettingState(World* world);
        ~SettingState() override;

        void update() override;
        void draw() override;

        void setIsTitleBefore(bool isTitleBefore);
};

#endif