#include "GameState/TitleScreenState.h"
#include "GameState/PlayingState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"
#include "SaveGame.h"
#include "raylib.h"

TitleScreenState::TitleScreenState(World* world)
    : GameState(world, GameStateType::TITLE_SCREEN),
    continueButton({1600 / 2 - 100, 900 / 2 + 50, 200, 50}, "Continue", 20),
    newGameButton({1600 / 2 - 100, 900 / 2 + 110, 200, 50}, "New Game", 20),
    optionsButton({1600 / 2 - 100, 900 / 2 + 170, 200, 50}, "Options", 20),
    exitButton({1600 / 2 - 100, 900 / 2 + 230, 200, 50}, "Exit", 20) {
        if(SaveGame::saveGameExists()) {
            isSavedGameAvailable = true;
        }
        else {
            isSavedGameAvailable = false;
            newGameButton.setPosition({1600 / 2 - 100, 900 / 2 + 50});
            optionsButton.setPosition({1600 / 2 - 100, 900 / 2 + 110});
            exitButton.setPosition({1600 / 2 - 100, 900 / 2 + 170});
        }
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
    if(IsKeyPressed(KEY_ENTER)) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        world->setGameState(new ChooseCharacterState(world));
    }
    if(newGameButton.isPressed()) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        world->setGameState(new ChooseCharacterState(world));
    }
    else if(continueButton.isPressed() && isSavedGameAvailable) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        SaveGame::loadGame(world);
    }
    else if(exitButton.isPressed()) {
        CloseWindow();
    }
    else if(optionsButton.isPressed()) {
        SettingState* newState = new SettingState(world);
        newState->setStateBeforeSetting(GameStateType::TITLE_SCREEN);
        world->setGameState(newState);
    }
}

void TitleScreenState::draw() {
    ClearBackground(RAYWHITE);
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    DrawTexture(texture["Logo"], GetScreenWidth() / 2 - texture["Logo"].width / 2, GetScreenHeight() / 2 - texture["Logo"].height / 2 - 200, WHITE);
    DrawTexture(texture["TitleScreenBackground"], 0, 0, WHITE);
    newGameButton.draw();
    if(isSavedGameAvailable) {
        continueButton.draw();
    }
    optionsButton.draw();
    exitButton.draw();
}