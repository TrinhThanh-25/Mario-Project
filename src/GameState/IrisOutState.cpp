#include "GameState/IrisOutState.h"
#include "GameState/GoNextMapState.h"
#include "Common/ResourceManager.h"

IrisOutState::IrisOutState(World* world) 
    : GameState(world),
      gameHud(world->getGameHud()) {
    
}

IrisOutState::~IrisOutState() {

}

void IrisOutState::update() {
    if(!IsMusicStreamPlaying(ResourceManager::getMusic()["CourseClear"])) {
        PlaySound(ResourceManager::getSound()["GoalIrisOut"]);
        world->setGameState(new GoNextMapState(world));
    }
    else {
        UpdateMusicStream(ResourceManager::getMusic()["CourseClear"]);
        if ((int)GetMusicTimeLength(ResourceManager::getMusic()["CourseClear"]) == (int)GetMusicTimePlayed(ResourceManager::getMusic()["CourseClear"])) {
            StopMusicStream(ResourceManager::getMusic()["CourseClear"]);
        }
    }
}

void IrisOutState::draw() {
    gameHud->draw();
    // Draw logic for IrisOutState
    // This could include rendering the game world, characters, UI elements, etc.
}