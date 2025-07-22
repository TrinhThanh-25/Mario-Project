#include "GameState/TitleScreenState.h"
#include "GameState/PlayingState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"
#include "SaveGame.h"
#include "raylib.h"

TitleScreenState::TitleScreenState(World* world)
    : GameState(world, GameStateType::TITLE_SCREEN),
    continueButton({1600 / 2 - 240, 900 / 2 + 110, 480, 50}, "Continue", 40),
    onePlayerGameButton({1600 / 2 - 240, 900 / 2 + 170, 480, 50}, "1 Player Game", 40),
    twoPlayersGameButton({1600 / 2 - 240, 900 / 2 + 230, 480, 50}, "2 Players Game", 40),
    optionsButton({1600 / 2 - 240, 900 / 2 + 290, 480, 50}, "Options", 40),
    exitButton({1600 / 2 - 240, 900 / 2 + 350, 480, 50}, "Exit", 40) {
        if(SaveGame::saveGameExists()) {
            isSavedGameAvailable = true;
            continueButton.Selected();
        }
        else {
            isSavedGameAvailable = false;
            onePlayerGameButton.Selected();
            onePlayerGameButton.setPosition({1600 / 2 - 220, 900 / 2 + 110});
            twoPlayersGameButton.setPosition({1600 / 2 - 220, 900 / 2 + 170});
            optionsButton.setPosition({1600 / 2 - 220, 900 / 2 + 230});
            exitButton.setPosition({1600 / 2 - 220, 900 / 2 + 290});
        }
}

TitleScreenState::~TitleScreenState() {

}

void TitleScreenState::update() {
    if(continueButton.isHovered() && isSavedGameAvailable ) {
        continueButton.Selected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
    }
    else if(onePlayerGameButton.isHovered()) {
        onePlayerGameButton.Selected();
        continueButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
    }
    else if(twoPlayersGameButton.isHovered()) {
        twoPlayersGameButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
    }
    else if(optionsButton.isHovered()) {
        optionsButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        exitButton.deSelected();
    }
    else if(exitButton.isHovered()) {
        exitButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
    }
    if(IsKeyPressed(KEY_DOWN)){
        if(continueButton.isSelected()) {
            continueButton.deSelected();
            onePlayerGameButton.Selected();
        }
        else if(onePlayerGameButton.isSelected()) {
            onePlayerGameButton.deSelected();
            twoPlayersGameButton.Selected();
        }
        else if(twoPlayersGameButton.isSelected()) {
            twoPlayersGameButton.deSelected();
            optionsButton.Selected();
        }
        else if(optionsButton.isSelected()) {
            optionsButton.deSelected();
            exitButton.Selected();
        }
        else if(exitButton.isSelected()) {
            exitButton.deSelected();
            if(isSavedGameAvailable) {
                continueButton.Selected();
            }
            else {
                onePlayerGameButton.Selected();
            }
        }
    }
    else if(IsKeyPressed(KEY_UP)){
        if(continueButton.isSelected()) {
            continueButton.deSelected();
            exitButton.Selected();
        }
        else if(onePlayerGameButton.isSelected()) {
            onePlayerGameButton.deSelected();
            if (isSavedGameAvailable) {
                continueButton.Selected();
            }
            else {
                exitButton.Selected();
            }
        }
        else if(twoPlayersGameButton.isSelected()) {
            twoPlayersGameButton.deSelected();
            onePlayerGameButton.Selected();
        }
        else if(optionsButton.isSelected()) {
            optionsButton.deSelected();
            twoPlayersGameButton.Selected();
        }
        else if(exitButton.isSelected()) {
            exitButton.deSelected();
            optionsButton.Selected();
        }
    }
    if(!IsMusicStreamPlaying(ResourceManager::getMusic()["Title"])) {
        PlayMusicStream(ResourceManager::getMusic()["Title"]);
    }
    else {
        UpdateMusicStream(ResourceManager::getMusic()["Title"]);
    }
    if(onePlayerGameButton.isPressed() || (IsKeyPressed(KEY_ENTER) && onePlayerGameButton.isSelected())) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        ChooseCharacterState* newState = new ChooseCharacterState(world);
        newState->setModeWorld(ModeWorld::SINGLEPLAYER);
        world->setGameState(newState);
    }
    else if(twoPlayersGameButton.isPressed() || (IsKeyPressed(KEY_ENTER) && twoPlayersGameButton.isSelected())) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        ChooseCharacterState* newState = new ChooseCharacterState(world);
        newState->setModeWorld(ModeWorld::MULTIPLAYER);
        world->setGameState(newState);
    }
    else if((continueButton.isPressed() || (IsKeyPressed(KEY_ENTER) && continueButton.isSelected())) && isSavedGameAvailable) {
        StopMusicStream(ResourceManager::getMusic()["Title"]);
        SaveGame::loadGame(*world);
    }
    else if(exitButton.isPressed() || (IsKeyPressed(KEY_ENTER) && exitButton.isSelected())) {
        world->setIsClosed(true);
    }
    else if(optionsButton.isPressed() || (IsKeyPressed(KEY_ENTER) && optionsButton.isSelected())) {
        SettingState* newState = new SettingState(world);
        newState->setStateBeforeSetting(GameStateType::TITLE_SCREEN);
        world->setGameState(newState);
    }
}

void TitleScreenState::draw() {
    ClearBackground(RAYWHITE);
    std::unordered_map<std::string, Texture2D>& texture = ResourceManager::getTexture();
    DrawTexture(texture["TitleScreenBackground"], 0, 0, WHITE);
    onePlayerGameButton.draw();
    twoPlayersGameButton.draw();
    if(isSavedGameAvailable) {
        continueButton.draw();
    }
    optionsButton.draw();
    exitButton.draw();
}