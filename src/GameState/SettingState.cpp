#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/TimeUpState.h"
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
    }
    else if(returnButton.isPressed()) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(*world);
        }
        world->setGameState(new TitleScreenState(world));
    }
    else if(exitButton.isPressed()) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(*world);
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

json SettingState::saveToJson() const {
    json j;
    GameState* tempState = nullptr;
    switch (stateBeforeSetting) {
        case GameStateType::COUNTING_POINT:
            tempState = new CountingPointState(world);
            break;
        case GameStateType::GO_NEXT_MAP:
            tempState = new GoNextMapState(world);
            break;
        case GameStateType::IRIS_OUT:
            tempState = new IrisOutState(world);
            break;
        case GameStateType::TIME_UP:
            tempState = new TimeUpState(world);
            break;
        case GameStateType::PLAYING:
            tempState = new PlayingState(world);
            break;
        default:
            tempState = nullptr;
            break;
    }
    if (tempState) {
        j = tempState->saveToJson();
        delete tempState;
    }
    return j;
}