#include "GameState/GoNextMapState.h"
#include "GameState/PlayingState.h"
#include "GameState/FinishedState.h"
#include "GameState/SettingState.h"
#include "Common/ResourceManager.h"
#include "Character/Character.h"

GoNextMapState::GoNextMapState(World* world)
    : GameState(world, GameStateType::GO_NEXT_MAP), 
    irisOutTime(1.0f), 
    irisOutAcum(0.0f), 
    isIrisOutFinished(false), 
    map(world->getMap()), 
    characters(world->getCharacters()),
    gameHud(world->getGameHud()),
    camera(world->getCamera()) {
}

GoNextMapState::~GoNextMapState() {

}

void GoNextMapState::update() {
    if(IsKeyPressed(KEY_ESCAPE)) {
        SettingState* settingState = new SettingState(world);
        settingState->setStateBeforeSetting(GameStateType::GO_NEXT_MAP);
        world->setGameState(settingState);
        return;
    }
    float deltaTime = GetFrameTime();
    irisOutAcum += deltaTime;
    if (irisOutAcum >= irisOutTime) {
        isIrisOutFinished = true;
    } 
    if(isIrisOutFinished) {
        world->nextMap();
    }
}

void GoNextMapState::draw() {
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
    float remainingTimeWidth = ResourceManager::getSmallNumberWidth( gameHud->getRemainingTime() );\
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

    RenderTexture2D mask = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    std::vector<Vector2> center;

    for (auto& character : characters) {
        center.push_back(GetWorldToScreen2D({character->getX() + character->getWidth() / 2.0f, character->getY() + character->getHeight() / 2.0f}, *camera));
    }

    float maxRadius = sqrt(GetScreenWidth() * GetScreenWidth() + GetScreenHeight() * GetScreenHeight());
    float irisRadius = maxRadius * (1 - irisOutAcum / irisOutTime);

    BeginTextureMode(mask);
    ClearBackground(BLACK);

    for (int i = 0; i < center.size(); ++i) {
        DrawCircleV(center[i], irisRadius, WHITE);
    }
    EndTextureMode();

    BeginBlendMode(BLEND_MULTIPLIED);
    DrawTextureRec(mask.texture, { 0, 0, (float)mask.texture.width, -(float)mask.texture.height }, { 0, 0 }, WHITE);
    EndBlendMode();
}

json GoNextMapState::saveToJson() const {
    json j = GameState::saveToJson();
    j["irisOutTime"] = irisOutTime;
    j["irisOutAcum"] = irisOutAcum;
    j["isIrisOutFinished"] = isIrisOutFinished;
    return j;
}

void GoNextMapState::loadFromJson(const json& j) {
    GameState::loadFromJson(j);
    irisOutTime = j["irisOutTime"].get<float>();
    irisOutAcum = j["irisOutAcum"].get<float>();
    isIrisOutFinished = j["isIrisOutFinished"].get<bool>();
}