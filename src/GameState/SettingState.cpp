#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/TimeUpState.h"
#include "Common/ResourceManager.h"
#include "SaveGame.h"

SettingState::SettingState(World* world)
    : GameState(world, GameStateType::SETTING),
    resumeButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 50, 300, 50}, "Resume", 36),
    restartButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 10, 300, 50}, "Restart", 36),
    returnButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 70, 300, 50}, "Return", 36),
    exitButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 130, 300, 50}, "Exit", 36) {
}

SettingState::~SettingState() {
    
}

void SettingState::update() {
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.update();
        exitButton.update();
        if(!IsMusicStreamPlaying(ResourceManager::getMusic()["Title"])) {
            PlayMusicStream(ResourceManager::getMusic()["Title"]);
        }
        else {
            UpdateMusicStream(ResourceManager::getMusic()["Title"]);
        }
    } else {
        resumeButton.update();
        restartButton.update();
        returnButton.update();
        exitButton.update();
    }
    if(resumeButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        world->setGameState(new PlayingState(world));
    }
    else if(restartButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        world->resetMap();
    }
    else if(returnButton.isPressed() || IsKeyPressed(KEY_ESCAPE)) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(*world);
        }
        std::vector<Character*>& characters = world->getCharacters();
        for (Character* character : characters) {
            delete character;
        }
        characters.clear();
        world->resetGame();
    }
    else if(exitButton.isPressed()) {
        if(stateBeforeSetting != GameStateType::TITLE_SCREEN) {
            SaveGame::saveGame(*world);
        }
        world->setIsClosed(true);
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