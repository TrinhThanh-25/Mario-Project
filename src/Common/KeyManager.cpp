#include "Common/KeyManager.h"
#include "Character/Character.h"
#include "Common/GroupKey.h"
#include "raylib.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void KeyManager::initializeKeys() {
    for (const auto& mode : {ModePlayer::ONEPLAYER, ModePlayer::FIRSTPLAYER, ModePlayer::SECONDPLAYER}) {
        GroupKey groupKey;
        groupKey.setKey(mode);
        groupKeys[mode] = groupKey;
    }
}

bool KeyManager::setKey(ModePlayer modePlayer, const std::string& keyName, int keyValue) {
    groupKeys[modePlayer].getKeys()[keyName] = keyValue;
    return true;
}

std::unordered_map<std::string, int>& KeyManager::getKeys(ModePlayer modePlayer) {
    return groupKeys[modePlayer].getKeys();
}

bool KeyManager::isKeyConflicted(ModePlayer modePlayer, const std::string& keyName, int keyValue) {
    const auto& onePlayerKeys = groupKeys[ModePlayer::ONEPLAYER].getKeys();
    const auto& firstPlayerKeys = groupKeys[ModePlayer::FIRSTPLAYER].getKeys();
    const auto& secondPlayerKeys = groupKeys[ModePlayer::SECONDPLAYER].getKeys();
    
    if (modePlayer == ModePlayer::ONEPLAYER) {
        for (const auto& pair : onePlayerKeys) {
            if (pair.first != keyName && pair.second == keyValue) {
                return true;
            }
        }
    } else if (modePlayer == ModePlayer::FIRSTPLAYER) {
        for (const auto& pair : firstPlayerKeys) {
            if (pair.first != keyName && pair.second == keyValue) {
                return true;
            }
        }
        for (const auto& pair : secondPlayerKeys) {
            if (pair.second == keyValue) {
                return true;
            }
        }
    } else if (modePlayer == ModePlayer::SECONDPLAYER) {
        for (const auto& pair : secondPlayerKeys) {
            if (pair.first != keyName && pair.second == keyValue) {
                return true;
            }
        }
        for (const auto& pair : firstPlayerKeys) {
            if (pair.second == keyValue) {
                return true;
            }
        }
    }
    
    return false;
}

void KeyManager::setKeyManagerForCharacter(Character* character, ModePlayer modePlayer) {
    if (character) {
        character->setModePlayer(modePlayer);
    }
}

void KeyManager::loadCurrentKeyManager(std::string fileName) {
    std::ifstream file(fileName);
    if (!file) {
        std::cerr << "Could not open json file " << fileName << std::endl;
        return;
    }
    json j;
    file >> j;
    for (auto& [modePlayer, playerKeys] : j.items()) {
        ModePlayer mode = static_cast<ModePlayer>(std::stoi(modePlayer));
        GroupKey groupKey;
        for (const auto& key : playerKeys) {
            groupKey.getKeys()[key["keyName"].get<std::string>()] = static_cast<KeyboardKey>(key["keyValue"].get<int>());
        }
        groupKeys[mode] = groupKey;
    }
}

void KeyManager::saveCurrentKeyManager(std::string fileName) {
    json j;
    for (auto& [modePlayer, groupKey] : groupKeys) {
        json playerKeys = json::array();
        for (auto& keySave : groupKey.getKeys()) {
            playerKeys.push_back({{"keyName", keySave.first}, {"keyValue", keySave.second}});
        }
        j[(int)modePlayer] = playerKeys;
    }
    std::ofstream file(fileName);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    }
}

void KeyManager::setDefaultKeyManager(std::string fileName) {
    loadCurrentKeyManager(fileName);
}