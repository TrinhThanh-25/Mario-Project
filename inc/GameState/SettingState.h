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
        GamepadManager* gamepadManager;

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
        std::string curInputName;
        KeyboardKey curKeyValue;
        int curGamepadButtonValue;
        Rectangle curRect;
        bool isEditing = false;
        bool isEditingGamepad = false;

        bool isDefaultNotified = false;
        bool isConflictNotified = false;
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
        void updateGamepadControlSetting();
        void drawGamepadControlSetting();
        void updateRestoreDefaultButton();
        void drawInputButton(ModePlayer modePlayer, const std::string& inputName, int inputValue, float x, float y, float width, float height, bool isGamepad);
        std::string getKeyName(int keyValue);
        std::string getGamepadButtonName(int buttonValue);
        std::string getInputName(int inputValue, bool isGamepad);
        std::string getFunctionName(const std::string& keyName);
        void checkInputButtonClick(Vector2 mousePos, Rectangle buttonRect, ModePlayer curModePlayer, std::string inputName, bool isGamepad);
        bool isMouseInClippingArea() const;
        void drawRestoreDefaultButton();
        Rectangle getRestoreDefaultButtonRect() const;
        void updateConflictStatus();

        void updateConfirmDefaultSetting();
        void drawConfirmDefaultSetting();
        void updateConflictNotification();
        void drawConflictNotification();
};

#endif