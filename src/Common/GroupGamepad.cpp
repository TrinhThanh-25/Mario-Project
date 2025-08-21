#include "Common/GroupGamepad.h"
#include "raylib.h"
#include "Character/Character.h"

GroupGamepad::~GroupGamepad() {
    buttons.clear();
}

std::unordered_map<std::string, int>& GroupGamepad::getButtons() {
    return buttons;
}

const std::unordered_map<std::string, int>& GroupGamepad::getButtons() const {
    return buttons;
}

void GroupGamepad::setButton(ModePlayer modePlayer) {
    if (modePlayer == ModePlayer::ONEPLAYER) {
        buttons = {
            {"UP", GAMEPAD_BUTTON_RIGHT_FACE_DOWN},          // A button (jump)
            {"DOWN", GAMEPAD_BUTTON_RIGHT_TRIGGER_2},      // RT button (cúi)
            {"CONTROL", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},  // B button (control)
            {"SHIFT", GAMEPAD_BUTTON_LEFT_TRIGGER_2}       // LT button (chạy nhanh)
        };
    } else if (modePlayer == ModePlayer::FIRSTPLAYER) {
        buttons = {
            {"UP", GAMEPAD_BUTTON_RIGHT_FACE_DOWN},          // A button (jump)
            {"DOWN", GAMEPAD_BUTTON_RIGHT_TRIGGER_2},      // RT button (cúi)
            {"CONTROL", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},  // B button (control)
            {"SHIFT", GAMEPAD_BUTTON_LEFT_TRIGGER_2}       // LT button (chạy nhanh)
        };
    } else if (modePlayer == ModePlayer::SECONDPLAYER) {
        buttons = {
            {"UP", GAMEPAD_BUTTON_RIGHT_FACE_DOWN},          // A button (jump)
            {"DOWN", GAMEPAD_BUTTON_RIGHT_TRIGGER_2},      // RT button (cúi)
            {"CONTROL", GAMEPAD_BUTTON_RIGHT_FACE_RIGHT},  // B button (control)
            {"SHIFT", GAMEPAD_BUTTON_LEFT_TRIGGER_2}       // LT button (chạy nhanh)
        };
    }
}
