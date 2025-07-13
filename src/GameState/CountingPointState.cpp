#include "GameState/CountingPointState.h"
#include "GameState/IrisOutState.h"
#include "Common/ResourceManager.h"
#include "raylib.h"

CountingPointState::CountingPointState(World* world)
    : GameState(world), 
      remainTimePoint(world->getRemainTimePoint()), 
      gameHud(world->getGameHud()) {
}

CountingPointState::~CountingPointState() {

}

void CountingPointState::update() {
    if(!IsMusicStreamPlaying(ResourceManager::getMusic()["CourseClear"])) {
        PlayMusicStream(ResourceManager::getMusic()["CourseClear"]);
    } else {
        UpdateMusicStream(ResourceManager::getMusic()["CourseClear"]);
    }

    *remainTimePoint--;
    gameHud->addPoints(50);
    if(*remainTimePoint % 3 == 0) {
        PlaySound(ResourceManager::getSound()["Coin"]);
    }
    if(*remainTimePoint <= 0) {
        world->setGameState(new IrisOutState(world));
    }
}

void CountingPointState::draw() {
    gameHud->draw();
    // Draw logic for CountingPointState
    // This could include rendering the game world, characters, UI elements, etc.
}