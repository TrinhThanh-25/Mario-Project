#ifndef GROUPKEY_H
#define GROUPKEY_H

#include <unordered_map>
#include <string>

class Character;
enum class ModePlayer;

class GroupKey {
    private:
        std::unordered_map<std::string, int> keys;
    public:
        GroupKey() = default;
        ~GroupKey();

        std::unordered_map<std::string, int>& getKeys();
        void setKey(ModePlayer modePlayer);
};

#endif