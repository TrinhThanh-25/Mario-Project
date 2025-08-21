#ifndef KEYMANAGER_H
#define KEYMANAGER_H

#include "Common/GroupKey.h"
#include <unordered_map>

class Character;
enum class ModePlayer;

class KeyManager {
    private:
        std::unordered_map<ModePlayer, GroupKey> groupKeys;

        static KeyManager* keyManager;
        KeyManager() = default;
    public:
        static KeyManager* getInstance();
        KeyManager(const KeyManager&) = delete;
        KeyManager& operator=(const KeyManager&) = delete;
        ~KeyManager() = default;

        void initializeKeys();
        bool setKey(ModePlayer modePlayer, const std::string& keyName, int keyValue);
        std::unordered_map<std::string, int>& getKeys(ModePlayer modePlayer);
        bool isKeyConflicted(ModePlayer modePlayer, const std::string& keyName, int keyValue);
        ModePlayer getModePlayer(ModePlayer modePlayer) const;
        void setKeyManagerForCharacter(Character* character, ModePlayer modePlayer);

        void loadCurrentKeyManager(std::string fileName = "../resources/SaveGame/CurrentKeyManager.json");
        void saveCurrentKeyManager(std::string fileName = "../resources/SaveGame/CurrentKeyManager.json");
        void setDefaultKeyManager(std::string fileName = "../resources/SaveGame/DefaultKeyManager.json");
};

#endif