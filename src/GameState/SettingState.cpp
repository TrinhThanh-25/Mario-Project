#include "GameState/SettingState.h"
#include "GameState/PlayingState.h"
#include "GameState/TitleScreenState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/TimeUpState.h"
#include "Common/ResourceManager.h"
#include "Common/AudioManager.h"
#include "SaveGame.h"
#include "GameState/GameStateFactory.h"

SettingState::SettingState(World* world)
    : GameState(world, GameStateType::SETTING),
    resumeButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50, 300, 50}, "Resume", 36),
    restartButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110, 300, 50}, "Restart", 36),
    returnButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 170, 300, 50}, "Return", 36),
    exitButton({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 230, 300, 50}, "Exit", 36),
    musicVolumeSlider("MUSIC",{(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 - 50}, 300, 0.0f, 1.0f, AudioManager::getMusicVolume(), 36),
    sfxVolumeSlider("SFX",{(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 10}, 300, 0.0f, 1.0f, AudioManager::getSfxVolume(), 36),
    backgroundPositionx(0.0f), speed(40.0f) {
}

SettingState::~SettingState() {
    
}

void SettingState::update() {
    musicVolumeSlider.update();
    sfxVolumeSlider.update();
    AudioManager::setMusicVolume(musicVolumeSlider.getValue());
    AudioManager::setSfxVolume(sfxVolumeSlider.getValue());
    if(backgroundPositionx > 400) {
        speed *=(-1.0f);
    } else if(backgroundPositionx < 0) {
        speed *=(-1.0f);
    }
    float deltaTime = GetFrameTime();
    backgroundPositionx += speed * deltaTime;
    if(stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.update();
        exitButton.update();
    } else {
        resumeButton.update();
        restartButton.update();
        returnButton.update();
        exitButton.update();
    }
    if(resumeButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
            SaveGame::loadGame(*world);
        }
        else {
            SaveGame::loadGame(*world, "../resources/SaveGame/" + world->getMap()->getMapFileName() + ".json");
        }
    }
    else if(restartButton.isPressed() && stateBeforeSetting != GameStateType::TITLE_SCREEN) {
        world->resetMap();
    }
    else if(returnButton.isPressed() || IsKeyPressed(KEY_ESCAPE)) {
        world->resetGame();
    }
    else if(exitButton.isPressed()) {
        world->setIsClosed(true);
    }
}

void SettingState::draw() {
    DrawTextureRec(ResourceManager::getTexture()["SettingBackground"], 
        Rectangle{(float)backgroundPositionx, 0, (float)GetScreenWidth(), (float)GetScreenHeight()}, 
        Vector2{0, 0}, WHITE);
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
    musicVolumeSlider.draw();
    sfxVolumeSlider.draw();
}

void SettingState::setStateBeforeSetting(GameStateType stateBeforeSetting) {
    this->stateBeforeSetting = stateBeforeSetting;
    if (stateBeforeSetting == GameStateType::TITLE_SCREEN) {
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110});
    }
    else {
        resumeButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 50});
        restartButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 110});
        returnButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 170});
        exitButton.setPosition({(float)GetScreenWidth() / 2 - 150, (float)GetScreenHeight() / 2 + 230});
    }
}

json SettingState::saveToJson() const {
    json j;
    GameState* tempState = GameStateFactory::createGameState(world, stateBeforeSetting);
    if (tempState) {
        j = tempState->saveToJson();
        delete tempState;
        tempState = nullptr;
    }
    return j;
}