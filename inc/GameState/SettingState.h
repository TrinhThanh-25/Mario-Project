#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "GUI/Button.h"

class SettingState : public GameState {
    private:
        GameStateType stateBeforeSetting;

        Button resumeButton;
        Button restartButton;
        Button returnButton;
        Button exitButton;
        // music / sfx
    public:
        SettingState(World* world);
        ~SettingState() override;

        void update() override;
        void draw() override;

        void setStateBeforeSetting(GameStateType stateBeforeSetting);

        json saveToJson() const override;
};

#endif