#ifndef GAMEPADMANAGER_H
#define GAMEPADMANAGER_H

#include "Common/GroupGamepad.h"
#include <unordered_map>

class Character;
enum class ModePlayer;

class GamepadManager {
    private:
        std::unordered_map<ModePlayer, GroupGamepad> groupGamepads;

        static GamepadManager* gamepadManager;
        GamepadManager() = default;
        
    public:
        static GamepadManager* getInstance();
        GamepadManager(const GamepadManager&) = delete;
        GamepadManager& operator=(const GamepadManager&) = delete;
        ~GamepadManager() = default;

        void initializeButtons();
        void setButton(ModePlayer modePlayer, const std::string& buttonName, int buttonValue);
        std::unordered_map<std::string, int>& getButtons(ModePlayer modePlayer);
        bool isButtonConflicted(ModePlayer modePlayer, const std::string& buttonName, int buttonValue);
        void setGamepadManagerForCharacter(Character* character, ModePlayer modePlayer);

        bool isJoystickMovingLeft(int gamepadID) const;
        bool isJoystickMovingRight(int gamepadID) const;

        void loadCurrentGamepadManager(std::string fileName = "../resources/SaveGame/CurrentGamepadManager.json");
        void saveCurrentGamepadManager(std::string fileName = "../resources/SaveGame/CurrentGamepadManager.json");
        void setDefaultGamepadManager(std::string fileName = "../resources/SaveGame/DefaultGamepadManager.json");
};

#endif
