#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "Common/GroupKey.h"
#include <unordered_map>

class Character;
enum class ModePlayer;

class KeyManager {
    private:
        std::unordered_map<ModePlayer, GroupKey> groupKeys;
    public:
        KeyManager() = default;
        ~KeyManager() = default;

        void initializeKeys();
        void setKey(ModePlayer modePlayer, const std::string& keyName, int keyValue);
        std::unordered_map<std::string, int>& getKeys(ModePlayer modePlayer);
        ModePlayer getModePlayer(ModePlayer modePlayer) const;
        void setKeyManagerForCharacter(Character* character, ModePlayer modePlayer);
};

#endif