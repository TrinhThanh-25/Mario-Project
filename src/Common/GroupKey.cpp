#include "Common/GroupKey.h"
#include "raylib.h"
#include "Character/Character.h"

GroupKey::~GroupKey() {
    keys.clear();
}

std::unordered_map<std::string, int>& GroupKey::getKeys() {
    return keys;
}

const std::unordered_map<std::string, int>& GroupKey::getKeys() const {
    return keys;
}

void GroupKey::setKey(ModePlayer modePlayer) {
    if (modePlayer == ModePlayer::ONEPLAYER) {
        keys = {
            {"LEFT", KEY_LEFT},
            {"RIGHT", KEY_RIGHT},
            {"UP", KEY_SPACE},
            {"DOWN", KEY_DOWN},
            {"CONTROL", KEY_LEFT_CONTROL},
            {"SHIFT", KEY_LEFT_SHIFT}
        };
    } else if (modePlayer == ModePlayer::FIRSTPLAYER) {
        keys = {
            {"LEFT", KEY_A},
            {"RIGHT", KEY_D},
            {"UP", KEY_W},
            {"DOWN", KEY_S},
            {"CONTROL", KEY_LEFT_CONTROL},
            {"SHIFT", KEY_LEFT_SHIFT}
        };
    } else if (modePlayer == ModePlayer::SECONDPLAYER) {
        keys = {
            {"LEFT", KEY_LEFT},
            {"RIGHT", KEY_RIGHT},
            {"UP", KEY_UP},
            {"DOWN", KEY_DOWN},
            {"CONTROL", KEY_RIGHT_CONTROL},
            {"SHIFT", KEY_RIGHT_SHIFT}
        };
    }
}