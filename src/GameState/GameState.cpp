#include "GameState/GameState.h"

GameState::GameState(World* world, GameStateType type)
    : world(world), gameStateType(type) {
}

void GameState::enter() {
    
}

void GameState::exit() {
    
}

GameStateType GameState::getGameStateType() const {
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

// Common state management functions - default empty implementations
void GameState::setMapFileName(const std::string& mapFileName) {
    // Default empty implementation
}

void GameState::setMap(int width, int height, const std::vector<int>& mapGrid) {
    // Default empty implementation
}

void GameState::setIsSaved(bool saved) {
    // Default empty implementation
}

void GameState::setModeWorld(ModeWorld mode) {
    // Default empty implementation
}

void GameState::setStateBeforeSetting(GameStateType state) {
    // Default empty implementation
}

// Common map operations - default empty implementations
void GameState::loadMap() {
    // Default empty implementation
}

void GameState::saveMap() {
    // Default empty implementation
}

void GameState::clearMap() {
    // Default empty implementation
}

void GameState::applyMapSize() {
    // Default empty implementation
}

// Common file operations - default empty implementations
void GameState::load(const std::string& fileName) {
    // Default empty implementation
}

void GameState::save(const std::string& fileName) {
    // Default empty implementation
}

// Common utility functions - default empty implementations
void GameState::handleUnsavedWarning() {
    // Default empty implementation
}

void GameState::loadTileTextures() {
    // Default empty implementation
}

void GameState::createDefaultMapFile(const std::string& mapName) {
    // Default empty implementation
}

void GameState::saveHistory() {
    // Default empty implementation
}

void GameState::setCameraPosition(float x, float y) {
    // Default empty implementation
}