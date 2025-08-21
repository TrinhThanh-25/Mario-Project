#include "Common/GamepadManager.h"
#include "Character/Character.h"
#include "Common/GroupGamepad.h"
#include "raylib.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

GamepadManager* GamepadManager::gamepadManager = nullptr;

using json = nlohmann::json;

GamepadManager* GamepadManager::getInstance() {
    if (!gamepadManager) {
        gamepadManager = new GamepadManager();
    }
    return gamepadManager;
}

void GamepadManager::initializeButtons() {
    for (const auto& mode : {ModePlayer::ONEPLAYER, ModePlayer::FIRSTPLAYER, ModePlayer::SECONDPLAYER}) {
        GroupGamepad groupGamepad;
        groupGamepad.setButton(mode);
        groupGamepads[mode] = groupGamepad;
    }
}

void GamepadManager::setButton(ModePlayer modePlayer, const std::string& buttonName, int buttonValue) {
    auto& buttons = groupGamepads[modePlayer].getButtons();
    buttons[buttonName] = buttonValue;
}

std::unordered_map<std::string, int>& GamepadManager::getButtons(ModePlayer modePlayer) {
    return groupGamepads[modePlayer].getButtons();
}

bool GamepadManager::isButtonConflicted(ModePlayer modePlayer, const std::string& buttonName, int buttonValue) {
    const auto& onePlayerButtons = groupGamepads[ModePlayer::ONEPLAYER].getButtons();
    const auto& firstPlayerButtons = groupGamepads[ModePlayer::FIRSTPLAYER].getButtons();
    const auto& secondPlayerButtons = groupGamepads[ModePlayer::SECONDPLAYER].getButtons();
    
    if (modePlayer == ModePlayer::ONEPLAYER) {
        for (const auto& pair : onePlayerButtons) {
            if (pair.first != buttonName && pair.second == buttonValue) {
                return true;
            }
        }
    } else if (modePlayer == ModePlayer::FIRSTPLAYER) {
        for (const auto& pair : firstPlayerButtons) {
            if (pair.first != buttonName && pair.second == buttonValue) {
                return true;
            }
        }
    } else if (modePlayer == ModePlayer::SECONDPLAYER) {
        for (const auto& pair : secondPlayerButtons) {
            if (pair.first != buttonName && pair.second == buttonValue) {
                return true;
            }
        }
    }
    
    return false;
}

void GamepadManager::setGamepadManagerForCharacter(Character* character, ModePlayer modePlayer) {
    if (character) {
        character->setModePlayer(modePlayer);
    }
}

void GamepadManager::loadCurrentGamepadManager(std::string fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Could not open json file " << fileName << std::endl;
        return;
    }
    json j;
    file >> j;
    for (auto& [modePlayer, playerButtons] : j.items()) {
        ModePlayer mode = static_cast<ModePlayer>(std::stoi(modePlayer));
        GroupGamepad groupGamepad;
        for (const auto& button : playerButtons) {
            groupGamepad.getButtons()[button["buttonName"].get<std::string>()] = button["buttonValue"].get<int>();
        }
        groupGamepads[mode] = groupGamepad;
    }
}

void GamepadManager::saveCurrentGamepadManager(std::string fileName) {
    json j;
    for (auto& [modePlayer, groupGamepad] : groupGamepads) {
        json playerButtons = json::array();
        for (auto& buttonSave : groupGamepad.getButtons()) {
            playerButtons.push_back({{"buttonName", buttonSave.first}, {"buttonValue", buttonSave.second}});
        }
        j[(int)modePlayer] = playerButtons;
    }
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    }
}

void GamepadManager::setDefaultGamepadManager(std::string fileName) {
    loadCurrentGamepadManager(fileName);
}

bool GamepadManager::isJoystickMovingLeft(int gamepadID) const {
    if (!IsGamepadAvailable(gamepadID)) return false;
    float axisX = GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X);
    return axisX < -0.2f;
}

bool GamepadManager::isJoystickMovingRight(int gamepadID) const {
    if (!IsGamepadAvailable(gamepadID)) return false;
    float axisX = GetGamepadAxisMovement(gamepadID, GAMEPAD_AXIS_LEFT_X);
    return axisX > 0.2f;
}
