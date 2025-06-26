#include "GameState/TitleScreenState.h"
#include "GameState/PlayingState.h"
#include "Common/ResourceManager.h"
#include "raylib.h"

TitleScreenState::TitleScreenState(World* world)
    : GameState(world) {
}

TitleScreenState::~TitleScreenState() {

}

void TitleScreenState::update() {
    if(!IsMusicStreamPlaying(ResourceManager::getMusic()["Title"])) {
        PlayMusicStream(ResourceManager::getMusic()["Title"]);
    }
    else {
        UpdateMusicStream(ResourceManager::getMusic()["Title"]);
    }
    if(GetKeyPressed()){
        world->setGameState(new PlayingState(world));
    }
}

void TitleScreenState::draw() {

}

void TitleScreenState::enter() {
    PlayMusicStream(ResourceManager::getMusic()["Title"]);
}

void TitleScreenState::exit() {
    StopMusicStream(ResourceManager::getMusic()["Title"]);
}