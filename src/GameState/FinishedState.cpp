#include "GameState/FinishedState.h"
#include "Common/ResourceManager.h"

FinishedState::FinishedState(World* world)
    : GameState(world), 
      gameHud(world->getGameHud()) {

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
        StopMusicStream(ResourceManager::getMusic()["CourseClear"]);
        world->resetGame();
    }
}

void FinishedState::draw() {
    gameHud->draw();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    // in cái gì đó ra cũng được ( texture hay dòng nhắn )
}