#include "GameState/SettingState.h"

SettingState::SettingState(World* world)
    : GameState(world) {
}

SettingState::~SettingState() {
    // Cleanup if necessary
}

void SettingState::update() {
    // Update logic for the settings state
    // This could include handling user input, updating settings, etc.
}

void SettingState::draw() {
    // Draw logic for the settings state
    // This could include rendering the settings menu, options, etc.
    // Additional drawing code for settings UI can be added here
}