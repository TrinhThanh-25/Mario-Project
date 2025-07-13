#include "GameState/TimeUpState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"

TimeUpState::TimeUpState(World* world) 
    : GameState(world, GameStateType::TIME_UP) {
    
}

TimeUpState::~TimeUpState() {
    
}

void TimeUpState::update() {
    if(IsKeyPressed(KEY_ESCAPE)) {
        SettingState* settingState = new SettingState(world);
        settingState->setStateBeforeSetting(GameStateType::TIME_UP);
        world->setGameState(settingState);
        return;
    }
    world->resetWhenCharacterDead();
}

void TimeUpState::draw() {
    DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), BLACK );
    Texture2D* texture = &ResourceManager::getTexture()["GuiTimeUp"];
    DrawTexture(*texture, GetScreenWidth() / 2 - texture->width / 2, GetScreenHeight() / 2 - texture->height / 2, WHITE);
}