#ifndef SETTINGSTATE_H
#define SETTINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "GUI/Button.h"
#include "GUI/Slider.h"
#include "Character/Character.h" 

class SettingState : public GameState {
    private:
        Camera2D* camera;
        KeyManager* keyManager;

        GameStateType stateBeforeSetting;

        Slider musicVolumeSlider;
        Slider sfxVolumeSlider;

        Button resumeButton;
        Button restartButton;
        Button returnButton;
        Button exitButton;

        float backgroundPositionx;
        float speed;

        ModePlayer curModePlayer;
        std::string curKeyName;
        KeyboardKey curKeyValue;
        Rectangle curRect;
        bool isEditingKey = false;

        bool isDefaultNotified = false;
        bool isKeyConflictedNotified = false;
        bool isConflict = false;
        
    public:
        SettingState(World* world);
        ~SettingState() override;

        void update() override;
        void draw() override;

        void setStateBeforeSetting(GameStateType stateBeforeSetting);

        json saveToJson() const override;
    private:
        void updateVolumeAndButtonSetting();
        void drawVolumeAndButtonSetting();
        void updateKeyControlSetting();
        void drawKeyControlSetting();
        void updateRestoreDefaultButton();
        void drawKeyButton(ModePlayer modePlayer, const std::string& keyName, int keyValue, float x, float y, float width, float height);
        std::string getKeyName(int keyValue);
        std::string getFunctionName(const std::string& keyName);
        void checkKeyButtonClick(Vector2 mousePos, Rectangle buttonRect, ModePlayer curModePlayer, std::string curKeyName);
        bool isMouseInClippingArea() const;
        void drawRestoreDefaultButton();
        Rectangle getRestoreDefaultButtonRect() const;
        void updateConflictStatus();

        void updateConfirmDefaultSetting();
        void drawConfirmDefaultSetting();
        void updateKeyConflictNotification();
        void drawKeyConflictNotification();
};

#endif