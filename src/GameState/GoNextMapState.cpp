#include "GameState/GoNextMapState.h"
#include "GameState/PlayingState.h"
#include "GameState/FinishedState.h"

GoNextMapState::GoNextMapState(World* world)
    : GameState(world), 
    irisOutTime(1.0f), 
    irisOutAcum(0.0f), 
    isIrisOutFinished(false), 
    map(world->getMap()), 
    characters(world->getCharacters()),
    gameHud(world->getGameHud()) {
}

GoNextMapState::~GoNextMapState() {

}

void GoNextMapState::update() {
    float deltaTime = GetFrameTime();
    irisOutAcum += deltaTime;
    if (irisOutAcum >= irisOutTime) {
        isIrisOutFinished = true;
    } 
    if(isIrisOutFinished) {
        world->nextMap();
    }
}

void GoNextMapState::draw() {
    gameHud->draw();
    // Draw logic for GoNextMapState
    // This could include rendering the game world, characters, UI elements, etc.
}