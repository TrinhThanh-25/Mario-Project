#include "Game/World.h"
#include "GameState/TitleScreenState.h"
#include "GameState/ChooseCharacterState.h"
#include "GameState/PlayingState.h"
#include "GameState/CountingPointState.h"
#include "GameState/FinishedState.h"
#include "GameState/GameOverState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/SettingState.h"
#include "GameState/TimeUpState.h"


float World::gravity = 1200.0f;

World::World(int width, int height, const std::string& title, int FPS)
    : map (nullptr, this, 1), camera(),
    width(width), height(height), title(title), FPS(FPS) {
        characters.push_back(new Mario( ModePlayer::FIRSTPLAYER, {64, 0}, {32, 40}, {0, 0}, BLUE, 260.0f, 360.0f, -600.0f));
        characters.push_back(new Mario( ModePlayer::SECONDPLAYER, {64, 0}, {32, 40}, {0, 0}, GREEN, 260.0f, 360.0f, -600.0f));
        map.setCharacters(&characters);
        modeWorld = ModeWorld::SINGLEPLAYER;
        gameState = new TitleScreenState(this);
}

World::~World() {
    for (Character* character : characters) {
        delete character;
    }
    characters.clear();
    if (gameState != nullptr) {
        gameState->exit();
        delete gameState;
    }
}

void World::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
        // SetConfigFlags( FLAG_FULLSCREEN_MODE );
        // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    SetConfigFlags(FLAG_WINDOW_ALWAYS_RUN);
    InitWindow(width, height, title.c_str());
    SetWindowIcon(LoadImage("../resources/icon.png"));
    SetExitKey(0);
    InitAudioDevice();
    SetTargetFPS(FPS);
}

void World::update() {
    gameState->update();
}

void World::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    BeginMode2D(camera);
    gameState->draw();
    EndMode2D();

    DrawFPS(10, 10);
    EndDrawing();
}

void World::updateCamera() {
    camera.target = {0, 0};
    camera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() - 104};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
    setCamera(&camera);
}

void World::setGameState(GameState* newState) {
    if (gameState != nullptr) {
        gameState->exit();
    }
    delete gameState;
    gameState = newState;
    if (gameState != nullptr) {
        gameState->enter();
    }
}

void World::setCamera(Camera2D *camera) {
    map.setCamera(camera);
}

Camera2D* World::getCamera() {
    return &camera;
}

std::vector<Character*>& World::getCharacters() {
    return characters;
}

Map* World::getMap() {
    return &map;
}

ModeWorld* World::getModeWorld() {
    return &modeWorld;
}