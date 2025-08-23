#include "GameState/GameOverState.h"
#include "Common/ResourceManager.h"
#include "SaveGame.h"
#include "GameState/GameStateFactory.h"

GameOverState::GameOverState(World* world)
    : GameState(world, GameStateType::GAME_OVER) {
}

GameOverState::~GameOverState() {
    
}

void GameOverState::update() {
    world->playGameOverMusic();
    world->resetWhenCharacterDead();
    SaveGame::deleteSaveGame();
    if(IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_ENTER)) {
        world->stopPlayerDownMusic();
        world->stopGameOverMusic();
        world->resetGame();
        return;
    }
}

void GameOverState::draw() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    Texture2D texture = ResourceManager::getTexture()["GuiGameOver"];
    DrawTexture(texture, GetScreenWidth() / 2 - texture.width / 2, GetScreenHeight() / 2 - texture.height / 2, WHITE);
}