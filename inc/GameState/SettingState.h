#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "GUI/Button.h"
#include "GUI/Slider.h" 

class SettingState : public GameState {
    private:
        GameStateType stateBeforeSetting;

        // Slider musicVolumeSlider;
        // Slider sfxVolumeSlider;

        Button resumeButton;
        Button restartButton;
        Button returnButton;
        Button exitButton;
    public:
        SettingState(World* world);
        ~SettingState() override;

        void update() override;
        void draw() override;

        void setStateBeforeSetting(GameStateType stateBeforeSetting);

        json saveToJson() const override;
};

#endif