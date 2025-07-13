#include "GameState/FinishedState.h"
#include "Common/ResourceManager.h"

FinishedState::FinishedState(World* world)
    : GameState(world, GameStateType::FINISHED) {

}

FinishedState::~FinishedState() {

}

void FinishedState::update() {
    if (!IsMusicStreamPlaying(ResourceManager::getMusic()["Ending"])) {
        PlayMusicStream(ResourceManager::getMusic()["Ending"]);
    } else {
        UpdateMusicStream(ResourceManager::getMusic()["Ending"]);
    }

    if (IsKeyPressed(KEY_ENTER)) {
        world->resetGame();
    }
}

void FinishedState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.8f));
    ResourceManager::drawString("Congratulations!", GetScreenWidth() / 2 - ResourceManager::getDrawStringWidth("Congratulations!") / 2, GetScreenHeight() / 2 - ResourceManager::getDrawStringHeight() / 2);
    ResourceManager::drawString("Press enter to continue", GetScreenWidth() / 2 - ResourceManager::getDrawStringWidth("Press any key to continue") / 2, GetScreenHeight() / 2 + ResourceManager::getDrawStringHeight() / 2);
}