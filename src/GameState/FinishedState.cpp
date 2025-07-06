#include "GameState/FinishedState.h"
#include "Common/ResourceManager.h"

FinishedState::FinishedState(World* world)
    : GameState(world) {

}

FinishedState::~FinishedState() {

}

void FinishedState::update() {
    if (!IsMusicStreamPlaying(ResourceManager::getMusic()["Ending"])) {
        PlayMusicStream(ResourceManager::getMusic()["Ending"]);
    } else {
        UpdateMusicStream(ResourceManager::getMusic()["Ending"]);
    }

    if (GetKeyPressed()) {
        world->resetGame();
    }
}

void FinishedState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
    ResourceManager::drawString("Congratulations!", GetScreenWidth() / 2 - ResourceManager::getDrawStringWidth("Congratulations!") / 2, GetScreenHeight() / 2 - ResourceManager::getDrawStringHeight() / 2);
    ResourceManager::drawString("Press any key to continue", GetScreenWidth() / 2 - ResourceManager::getDrawStringWidth("Press any key to continue") / 2, GetScreenHeight() / 2 + ResourceManager::getDrawStringHeight() / 2);
}