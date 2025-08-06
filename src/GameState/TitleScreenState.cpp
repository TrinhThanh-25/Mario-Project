#include "GameState/TitleScreenState.h"
#include "GameState/PlayingState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"
#include "GameState/ChooseCustomizedMap.h"
#include "SaveGame.h"
#include "raylib.h"

TitleScreenState::TitleScreenState(World* world)
    : GameState(world, GameStateType::TITLE_SCREEN),
    continueButton({1600 / 2 - 260, 900 / 2 + 100, 520, 50}, "Continue", 40),
    onePlayerGameButton({1600 / 2 - 260, 900 / 2 + 160, 520, 50}, "1 Player Game", 40),
    twoPlayersGameButton({1600 / 2 - 260, 900 / 2 + 220, 520, 50}, "2 Player Game", 40),
    listMapButton({1600 / 2 - 260, 900 / 2 + 280, 520, 50}, "List Custom Maps", 40),
    optionsButton({1600 / 2 - 260, 900 / 2 + 340, 520, 50}, "Options", 40),
    exitButton({1600 / 2 - 260, 900 / 2 + 400, 520, 50}, "Exit", 40) {
        if(SaveGame::saveGameExists()) {
            isSavedGameAvailable = true;
            continueButton.Selected();
        }
        else {
            isSavedGameAvailable = false;
            onePlayerGameButton.Selected();
            onePlayerGameButton.setPosition({1600 / 2 - 220, 900 / 2 + 100});
            twoPlayersGameButton.setPosition({1600 / 2 - 220, 900 / 2 + 160});
            listMapButton.setPosition({1600 / 2 - 220, 900 / 2 + 220});
            optionsButton.setPosition({1600 / 2 - 220, 900 / 2 + 280});
            exitButton.setPosition({1600 / 2 - 220, 900 / 2 + 340});
        }
}

TitleScreenState::~TitleScreenState() {

}

void TitleScreenState::update() {
    continueButton.update();
    onePlayerGameButton.update();
    twoPlayersGameButton.update();
    listMapButton.update();
    optionsButton.update();
    exitButton.update();
    if(continueButton.isHovered() && isSavedGameAvailable && !continueButton.isSelected()) {
        continueButton.Selected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
        listMapButton.deSelected();
    }
    else if(onePlayerGameButton.isHovered() && !onePlayerGameButton.isSelected()) {
        onePlayerGameButton.Selected();
        continueButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
        listMapButton.deSelected();
    }
    else if(twoPlayersGameButton.isHovered() && !twoPlayersGameButton.isSelected()) {
        twoPlayersGameButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
        listMapButton.deSelected();
    }
    else if(listMapButton.isHovered() && !listMapButton.isSelected()) {
        listMapButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        exitButton.deSelected();
    }
    else if(optionsButton.isHovered() && !optionsButton.isSelected()) {
        optionsButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        exitButton.deSelected();
        listMapButton.deSelected();
    }
    else if(exitButton.isHovered() && !exitButton.isSelected()) {
        exitButton.Selected();
        continueButton.deSelected();
        onePlayerGameButton.deSelected();
        twoPlayersGameButton.deSelected();
        optionsButton.deSelected();
        listMapButton.deSelected();
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
            listMapButton.Selected();
        }
        else if(listMapButton.isSelected()) {
            listMapButton.deSelected();
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
        else if(listMapButton.isSelected()) {
            listMapButton.deSelected();
            twoPlayersGameButton.Selected();
        }
        else if(optionsButton.isSelected()) {
            optionsButton.deSelected();
            listMapButton.Selected();
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
        ChooseCharacterState* newState = new ChooseCharacterState(world);
        newState->setModeWorld(ModeWorld::SINGLEPLAYER);
        world->setGamePlay(GamePlay::PLAYDEVELOPEDMAP);
        world->setGameState(newState);
    }
    else if(twoPlayersGameButton.isPressed() || (IsKeyPressed(KEY_ENTER) && twoPlayersGameButton.isSelected())) {
        ChooseCharacterState* newState = new ChooseCharacterState(world);
        newState->setModeWorld(ModeWorld::MULTIPLAYER);
        world->setGamePlay(GamePlay::PLAYDEVELOPEDMAP);
        world->setGameState(newState);
    }
    else if((continueButton.isPressed() || (IsKeyPressed(KEY_ENTER) && continueButton.isSelected())) && isSavedGameAvailable) {
        world->setGamePlay(GamePlay::PLAYDEVELOPEDMAP);
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
    else if(listMapButton.isPressed() || (IsKeyPressed(KEY_ENTER) && listMapButton.isSelected())) {
        ChooseCustomizedMapState* newState = new ChooseCustomizedMapState(world);
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
    listMapButton.draw();
    optionsButton.draw();
    exitButton.draw();
}

void TitleScreenState::enter() {
    PlayMusicStream(ResourceManager::getMusic()["Title"]);
}

void TitleScreenState::exit() {
    StopMusicStream(ResourceManager::getMusic()["Title"]);
}