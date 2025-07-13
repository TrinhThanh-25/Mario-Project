#include "GameState/GameState.h"

GameState::GameState(World* world, GameStateType type)
    : world(world), gameStateType(type) {
}

void GameState::enter() {
    
}

void GameState::exit() {
    
}

GameStateType GameState::getType() const {
    return gameStateType;
}