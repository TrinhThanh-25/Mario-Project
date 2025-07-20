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

json GameState::saveToJson() const {
    json j;
    j["gameStateType"] = gameStateType;
    return j;
}

void GameState::loadFromJson(const json& j) {
    gameStateType = static_cast<GameStateType>(j["gameStateType"].get<int>());
}