#include "GameState/SettingState.h"

SettingState::SettingState(World* world)
    : GameState(world) {
}

SettingState::~SettingState() {
    // Cleanup if necessary
}

void SettingState::update() {
    // if press resume -> PlayingState (unless title -> setting)
    // if press quit -> TitleScreenState (unless title -> setting)
    // if press exit -> CloseWindow()
    // music / sfx
}

void SettingState::draw() {
    // draw those button and toggle (normal, hovered, pressed)
}