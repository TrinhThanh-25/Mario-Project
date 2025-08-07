#include "Common/KeyManager.h"
#include "Character/Character.h"
#include "Common/GroupKey.h"
#include "raylib.h"

void KeyManager::initializeKeys() {
    for (const auto& mode : {ModePlayer::ONEPLAYER, ModePlayer::FIRSTPLAYER, ModePlayer::SECONDPLAYER}) {
        GroupKey groupKey;
        groupKey.setKey(mode);
        groupKeys[mode] = groupKey;
    }
}

void KeyManager::setKey(ModePlayer modePlayer, const std::string& keyName, int keyValue) {
    if (groupKeys.find(modePlayer) != groupKeys.end()) {
        groupKeys[modePlayer].getKeys()[keyName] = keyValue;
    }
}

std::unordered_map<std::string, int>& KeyManager::getKeys(ModePlayer modePlayer) {
    return groupKeys[modePlayer].getKeys();
}

void KeyManager::setKeyManagerForCharacter(Character* character, ModePlayer modePlayer) {
    if (character) {
        character->setModePlayer(modePlayer);
        character->setKeyManager(this);
    }
}