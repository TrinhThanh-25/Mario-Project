#include "GameState/TimeUpState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"
#include "GameState/GameStateFactory.h"
#include "SaveGame.h"

TimeUpState::TimeUpState(World* world) 
    : GameState(world, GameStateType::TIME_UP) {
    
}

TimeUpState::~TimeUpState() {
    
}

void TimeUpState::update() {
    world->resetWhenCharacterDead();
    if(IsKeyPressed(KEY_ESCAPE)) {
        if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
            SaveGame::saveGame(*world);
        }
        else {
            SaveGame::saveGame(*world, "../resources/SaveGame/" + world->getMap()->getMapFileName() + ".json");
        }
        world->stopPlayerDownMusic();
        world->stopGameOverMusic();
        GameState* settingState = GameStateFactory::createGameState(world, GameStateType::SETTING);
        settingState->setStateBeforeSetting(GameStateType::PLAYING);
        world->setGameState(settingState);
        return;
    }
}

void TimeUpState::draw() {
    DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), BLACK );
    Texture2D* texture = &ResourceManager::getTexture()["GuiTimeUp"];
    DrawTexture(*texture, GetScreenWidth() / 2 - texture->width / 2, GetScreenHeight() / 2 - texture->height / 2, WHITE);
}