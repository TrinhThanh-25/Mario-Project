#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "SaveGame.h"

SettingState::SettingState(World* world)
    : GameState(world, GameStateType::SETTING),
    resumeButton({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50, 200, 50}, "Resume", 20),
    restartButton({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 10, 200, 50}, "Restart", 20),
    returnButton({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 70, 200, 50}, "Return", 20),
    exitButton({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 130, 200, 50}, "Exit", 20) {
}

SettingState::~SettingState() {
    
}

void SettingState::update() {
    if(resumeButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        world->setGameState(new PlayingState(world));
    }
    else if(restartButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        world->resetMap();
        world->setGameState(new PlayingState(world));
    }
    else if(returnButton.isPressed()) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(world);
        }
        world->setGameState(new TitleScreenState(world));
    }
    else if(exitButton.isPressed()) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(world);
        }
        CloseWindow();
    }
    // music / sfx
}

void SettingState::draw() {
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.draw();
        exitButton.draw();
    }
    else {
        resumeButton.draw();
        restartButton.draw();
        returnButton.draw();
        exitButton.draw();
    }
}

void SettingState::setStateBeforeSetting(GameStateType stateBeforeSetting) {
    this->stateBeforeSetting = stateBeforeSetting;
    if (stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 10});
    }
    else {
        resumeButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 - 50});
        restartButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 10});
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 70});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 100, (float)GetScreenHeight() / 2 + 130});
    }
}