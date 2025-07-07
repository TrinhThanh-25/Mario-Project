#include "GameState/TimeUpState.h"
#include "Common/ResourceManager.h"

TimeUpState::TimeUpState(World* world) : GameState(world) {
    
}

TimeUpState::~TimeUpState() {
    
}

void TimeUpState::update() {
    world->resetWhenCharacterDead();
}

void TimeUpState::draw() {
    DrawRectangle( 0, 0, GetScreenWidth(), GetScreenHeight(), BLACK );
    Texture2D* texture = &ResourceManager::getTexture()["GuiTimeUp"];
    DrawTexture(*texture, GetScreenWidth() / 2 - texture->width / 2, GetScreenHeight() / 2 - texture->height / 2, WHITE);
}