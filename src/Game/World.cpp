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
#include "Common/ResourceManager.h"
#include <unordered_map>


float World::gravity = 1200.0f;

World::World(int width, int height, const std::string& title, int FPS)
    : map(characters, this, 1), 
    camera(), 
    gameHud(this, 0, 0, 5, 0, 200.0f),
    width(width), 
    height(height), 
    title(title), 
    FPS(FPS),
    remainTimePoint(0), 
    gameState(nullptr), 
    modeWorld(ModeWorld::SINGLEPLAYER),
    playerDownMusicStreamPlaying(false),
    gameOverMusicStreamPlaying(false),
    pausedForTransition(false),
    pausedUpdateCharacters(false) {
        map.setCharacters(characters);
        modeWorld = ModeWorld::MULTIPLAYER;
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
    SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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
    if(playerDownMusicStreamPlaying) {
        playPlayerDownMusic();
    }
    else if (gameOverMusicStreamPlaying) {
        playGameOverMusic();
    }
}


void World::draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    gameState->draw();
    DrawFPS(10, 10);
    EndDrawing();
}

void World::updateCamera() {
    if (!characters.empty()) {
        camera.target = {
            characters[0]->getX() + characters[0]->getWidth() / 2.0f,
            characters[0]->getY() + characters[0]->getHeight() / 2.0f
        };
    } else {
        camera.target = {0, 0};
    }

    camera.offset = {(float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f};
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

void World::setModeWorld(ModeWorld mode) {
    modeWorld = mode;
}

ModeWorld* World::getModeWorld() {
    return &modeWorld;
}

std::vector<Character*>& World::getCharacters() {
    return characters;
}

Map* World::getMap() {
    return &map;
}

int* World::getRemainTimePoint() {
    return &remainTimePoint;
}

GameHud* World::getGameHud() {
    return &gameHud;
}

void World::playPlayerDownMusic() {
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    if (!playerDownMusicStreamPlaying) {
        playerDownMusicStreamPlaying = true;
    }
    if (!IsMusicStreamPlaying(music["PlayerDown"])) {
        PlayMusicStream(music["PlayerDown"]);
    } else {
        UpdateMusicStream(music["PlayerDown"]);
        if ((int)GetMusicTimeLength(music["PlayerDown"]) == (int)GetMusicTimePlayed(music["PlayerDown"])) {
            StopMusicStream(music["PlayerDown"]);
            playerDownMusicStreamPlaying = false;
        }
    }
}

void World::playGameOverMusic() {
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    if (!gameOverMusicStreamPlaying) {
        gameOverMusicStreamPlaying = true;
    }
    if (!IsMusicStreamPlaying(music["GameOver"])) {
        PlayMusicStream(music["GameOver"]);
    } else {
        UpdateMusicStream(music["GameOver"]);
        if ((int)GetMusicTimeLength(music["GameOver"]) == (int)GetMusicTimePlayed(music["GameOver"])) {
            StopMusicStream(music["GameOver"]);
            gameOverMusicStreamPlaying = false;
        }
    }
}

bool World::isPlayerDownMusicStreamPlaying() const {
    return playerDownMusicStreamPlaying;
}

bool World::isGameOverMusicStreamPlaying() const {
    return gameOverMusicStreamPlaying;
}

void World::pauseWorld(bool pausedForTransition, bool pausedUpdateCharacters) {
    this->pausedForTransition = pausedForTransition;
    this->pausedUpdateCharacters = pausedUpdateCharacters;
}

void World::resumeWorld() {
    pausedForTransition = false;
    pausedUpdateCharacters = false;
}

bool* World::getPausedForTransition() {
    return &pausedForTransition;
}

bool* World::getPausedUpdateCharacters() {
    return &pausedUpdateCharacters;
}

void World::resetMap() {
    for (Character* character : characters) {
        character->reset(true);
    }
    map.reset();
    gameHud.reset(true);
    setGameState(new PlayingState(this));
}

void World::resetGame() {
    for (Character* character : characters) {
        character->resetGame();
    }
    map.first();
    map.reset();
    gameHud.resetGame();
    setGameState(new TitleScreenState(this));
}

void World::nextMap() {
    if(map.next()) {
        for (Character* character : characters) {
            character->reset(false);
        }
        setGameState(new PlayingState(this));
    } else {
        setGameState(new FinishedState(this));
    }
}

void World::resetWhenCharacterDead() {
    if(!isPlayerDownMusicStreamPlaying() && !isGameOverMusicStreamPlaying()) {
        if(gameHud.getLives() > 0) {
            resetMap();
        }
        else if (gameHud.getLives() == 0) {
            playGameOverMusic();
            setGameState(new GameOverState(this));
            gameHud.setLives(-1);
        }
        else {
            resetGame();
        }
    }
}