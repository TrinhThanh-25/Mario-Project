#include "Game/World.h"
#include "Character/Character.h"
#include "GameState/GameState.h"
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
    // SetConfigFlags(FLAG_WINDOW_HIGHDPI);
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

json World::saveToJson() const {
    json j;
    j["characters"] = json::array();
    for (const auto& character : characters) {
        j["characters"].push_back(character->saveToJson());
    }
    j["map"] = map.saveToJson();
    j["camera"] = {
        {"target", {camera.target.x, camera.target.y}},
        {"offset", {camera.offset.x, camera.offset.y}},
        {"rotation", camera.rotation},
        {"zoom", camera.zoom}
    };
    j["gameHud"] = gameHud.saveToJson();
    j["remainTimePoint"] = remainTimePoint;
    j["gameState"] = gameState->saveToJson();
    j["modeWorld"] = static_cast<int>(modeWorld);
    j["playerDownMusicStreamPlaying"] = playerDownMusicStreamPlaying;
    j["gameOverMusicStreamPlaying"] = gameOverMusicStreamPlaying;
    j["pausedForTransition"] = pausedForTransition;
    j["pausedUpdateCharacters"] = pausedUpdateCharacters;
    return j;
}

void World::loadFromJson(const json& j) {
    characters.clear();
    for (const auto& characterJson : j["characters"]) {
        if (characterJson["name"] == "Mario") {
            Mario* mario = new Mario(
                static_cast<ModePlayer>(characterJson["modePlayer"].get<int>()),
                {characterJson["position"][0], characterJson["position"][1]},
                {characterJson["velocity"][0], characterJson["velocity"][1]},
                {characterJson["color"][0], characterJson["color"][1], characterJson["color"][2], characterJson["color"][3]},
                characterJson["speed"],
                characterJson["maxSpeed"],
                characterJson["jumpSpeed"]
            );
            mario->loadFromJson(characterJson);
            mario->setWorld(this);
            characters.push_back(mario);
        }
        else if( characterJson["name"] == "Luigi") {
            Luigi* luigi = new Luigi(
                static_cast<ModePlayer>(characterJson["modePlayer"].get<int>()),
                {characterJson["position"][0], characterJson["position"][1]},
                {characterJson["velocity"][0], characterJson["velocity"][1]},
                {characterJson["color"][0], characterJson["color"][1], characterJson["color"][2], characterJson["color"][3]},
                characterJson["speed"],
                characterJson["maxSpeed"],
                characterJson["jumpSpeed"]
            );
            luigi->loadFromJson(characterJson);
            luigi->setWorld(this);
            characters.push_back(luigi);
        }
    }
    map.loadFromJson(j["map"]);
    
    camera.target = {j["camera"]["target"][0], j["camera"]["target"][1]};
    camera.offset = {j["camera"]["offset"][0], j["camera"]["offset"][1]};
    camera.rotation = j["camera"]["rotation"];
    camera.zoom = j["camera"]["zoom"];
    
    gameHud.loadFromJson(j["gameHud"]);
    
    remainTimePoint = j["remainTimePoint"];
    
    gameState->loadFromJson(j["gameState"]);
    
    modeWorld = static_cast<ModeWorld>(j["modeWorld"].get<int>());
    playerDownMusicStreamPlaying = j["playerDownMusicStreamPlaying"];
    gameOverMusicStreamPlaying = j["gameOverMusicStreamPlaying"];
    pausedForTransition = j["pausedForTransition"];
    pausedUpdateCharacters = j["pausedUpdateCharacters"];
    
    switch (static_cast<GameStateType>(j["gameState"]["gameStateType"].get<int>())) {
        case GameStateType::TITLE_SCREEN:
            setGameState(new TitleScreenState(this));
            break;
        case GameStateType::CHOOSE_CHARACTER:
            setGameState(new ChooseCharacterState(this));
            break;
        case GameStateType::PLAYING:
            setGameState(new PlayingState(this));
            break;
        case GameStateType::COUNTING_POINT:
            setGameState(new CountingPointState(this));
            break;
        case GameStateType::FINISHED:
            setGameState(new FinishedState(this));
            break;
        case GameStateType::GAME_OVER:
            setGameState(new GameOverState(this));
            break;
        case GameStateType::GO_NEXT_MAP:
            setGameState(new GoNextMapState(this));
            break;
        case GameStateType::IRIS_OUT:
            setGameState(new IrisOutState(this));
            break;
        case GameStateType::SETTING:
            setGameState(new SettingState(this));
            break;
        case GameStateType::TIME_UP:
            setGameState(new TimeUpState(this));
            break;
    }
}