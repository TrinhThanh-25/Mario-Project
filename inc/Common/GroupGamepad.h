#ifndef GROUPGAMEPAD_H
#define GROUPGAMEPAD_H

#include <unordered_map>
#include <string>

class Character;
enum class ModePlayer;

class GroupGamepad {
    private:
        std::unordered_map<std::string, int> buttons;
    
    public:
        GroupGamepad() = default;
        ~GroupGamepad();

        std::unordered_map<std::string, int>& getButtons();
        const std::unordered_map<std::string, int>& getButtons() const;
        void setButton(ModePlayer modePlayer);
};

#endif
