#include "GameState/IrisOutState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"

IrisOutState::IrisOutState(World* world) 
    : GameState(world, GameStateType::IRIS_OUT),
      gameHud(world->getGameHud()), 
      map(world->getMap()),
      camera(world->getCamera()) {
    
}

IrisOutState::~IrisOutState() {

}

void IrisOutState::update() {
    if(IsKeyPressed(KEY_ESCAPE)) {
        SettingState* settingState = new SettingState(world);
        settingState->setStateBeforeSetting(GameStateType::IRIS_OUT);
        world->setGameState(settingState);
        return;
    }
    if (IsMusicStreamPlaying(ResourceManager::getMusic()["CourseClear"])) {
        UpdateMusicStream(ResourceManager::getMusic()["CourseClear"]);
        if ((int)GetMusicTimeLength(ResourceManager::getMusic()["CourseClear"]) == (int)GetMusicTimePlayed(ResourceManager::getMusic()["CourseClear"])) {
            StopMusicStream(ResourceManager::getMusic()["CourseClear"]);
        }
    } else {
        PlaySound(ResourceManager::getSound()["GoalIrisOut"]);
        world->setGameState(new GoNextMapState(world));
    }
}

void IrisOutState::draw() {
    BeginMode2D(*camera);
    map->draw();
    EndMode2D();
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(BLACK, 0.5f));
    gameHud->draw();

    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();

    float centerX = GetScreenWidth() / 2;
    float centerY = GetScreenHeight() / 2;
    DrawTexture( textures["GuiMario"], centerX - textures["GuiMario"].width / 2, centerY - 80, WHITE);

    std::string message1 = "course clear!";
    ResourceManager::drawString( message1, centerX - ResourceManager::getDrawStringWidth( message1 ) / 2, centerY - 40 );

    int totalTimePoints = gameHud->getRemainingTime() * 50;
    float clockWidth = textures["GuiClock"].width;
    float remainingTimeWidth = ResourceManager::getSmallNumberWidth( gameHud->getRemainingTime() );
    float multiplicationSignWidth = textures["GuiX"].width;
    float multiplierWidth = ResourceManager::getSmallNumberWidth( 50 );
    float equalSignWidth = ResourceManager::getDrawStringWidth( "=" );
    float totalTimePointsWidth = ResourceManager::getSmallNumberWidth( totalTimePoints );

    float resultWidth = clockWidth + remainingTimeWidth + multiplicationSignWidth + multiplierWidth + equalSignWidth + totalTimePointsWidth;
    float resultPositionX = centerX - resultWidth / 2;
    float resultPositionY = centerY;
    
    DrawTexture( textures["GuiClock"], resultPositionX, resultPositionY, WHITE );
    resultPositionX += clockWidth;
    ResourceManager::drawWhiteSmallNumber( gameHud->getRemainingTime(), resultPositionX, resultPositionY );
    resultPositionX += remainingTimeWidth;
    DrawTexture( textures["GuiX"], resultPositionX, resultPositionY, WHITE );
    resultPositionX += multiplicationSignWidth;
    ResourceManager::drawWhiteSmallNumber( 50, resultPositionX, resultPositionY );
    resultPositionX += multiplierWidth;
    ResourceManager::drawString( "=", resultPositionX, resultPositionY - 5 );
    resultPositionX += equalSignWidth;
    ResourceManager::drawWhiteSmallNumber( totalTimePoints, resultPositionX, resultPositionY );
    
    std::string message2 = "Total Points: " + std::to_string(gameHud->getPoints());
    ResourceManager::drawString( message2, centerX - ResourceManager::getDrawStringWidth( message2 ) / 2, centerY + 40 );
}

json IrisOutState::saveToJson() const {
    json j = GameState::saveToJson();
    return j;
}

void IrisOutState::loadFromJson(const json& j) {
    GameState::loadFromJson(j);
}