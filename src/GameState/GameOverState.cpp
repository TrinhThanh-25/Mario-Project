#include "GameState/GameOverState.h"
#include "Common/ResourceManager.h"
#include "SaveGame.h"

GameOverState::GameOverState(World* world)
    : GameState(world, GameStateType::GAME_OVER) {
}

GameOverState::~GameOverState() {
    
}

void GameOverState::update() {
    world->playGameOverMusic();
    world->resetWhenCharacterDead();
    SaveGame::deleteSaveGame();
}

void GameOverState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    Texture2D texture = ResourceManager::getTexture()["GuiGameOver"];
    DrawTexture(texture, GetScreenWidth() / 2 - texture.width / 2, GetScreenHeight() / 2 - texture.height / 2, WHITE);
}