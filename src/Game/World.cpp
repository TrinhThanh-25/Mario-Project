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
#include "GameState/CustomMapState.h"
#include "Common/ResourceManager.h"
#include "Character/CharacterFactory.h"
#include "GameState/TestMapState.h"
#include "GameState/ChooseCustomizedMap.h"
#include "GameState/GameStateFactory.h"
#include "Game/GameMode.h"
#include "Game/GamePlay.h"
#include <unordered_map>


float World::gravity = 1200.0f;

World::World(int width, int height, const std::string& title, int FPS)
    : map(characters, this, 1), 
    camera(), 
    gameHud(this, 0, 0, 0, 200.0f),
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
    pausedUpdateCharacters(false),
    GameLoop(),
    gameMode(GameMode::PLAYER),
    gamePlay(GamePlay::PLAYDEVELOPEDMAP) {
        keyManager.initializeKeys();
        gamepadManager.initializeButtons();
        map.setCharacters(characters);
        modeWorld = ModeWorld::MULTIPLAYER;
        gamePlay = GamePlay::PLAYCUSTOMMAP;
        gameState = new TitleScreenState(this);
        keyManager.loadCurrentKeyManager();
        gamepadManager.loadCurrentGamepadManager();
}

World::~World() {
    for (Character* character : characters) {
        if(character)
        delete character;
    }
    characters.clear();
    
    if (gameState) {
        gameState->exit();
        delete gameState;
    }
}

void World::init() {
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    // SetConfigFlags(FLAG_WINDOW_HIGHDPI);
    // SetConfigFlags( FLAG_FULLSCREEN_MODE );
    // SetConfigFlags( FLAG_WINDOW_UNDECORATED );
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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
    DrawFPS(10, 10);
    gameState->draw();
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
    if (gameState) {
        gameState->exit();
        delete gameState;
    }
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

KeyManager* World::getKeyManager() {
    return &keyManager;
}

GamepadManager* World::getGamepadManager() {
    return &gamepadManager;
}

void World::playPlayerDownMusic() {
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    if (!playerDownMusicStreamPlaying) {
        playerDownMusicStreamPlaying = true;
    }
    if (!IsMusicStreamPlaying(music["PlayerDown"])) {
        PlayMusicStream(music["PlayerDown"]);
        SeekMusicStream(music["PlayerDown"], 0.7f);
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

void World::stopPlayerDownMusic() {
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    if (IsMusicStreamPlaying(music["PlayerDown"])) {
        StopMusicStream(music["PlayerDown"]);
        playerDownMusicStreamPlaying = false;
    }
}

void World::stopGameOverMusic() {
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    if (IsMusicStreamPlaying(music["GameOver"])) {
        StopMusicStream(music["GameOver"]);
        gameOverMusicStreamPlaying = false;
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
    gameHud.reset();
    pausedForTransition = false;
    pausedUpdateCharacters = false;
    setGameState(new PlayingState(this));
}

void World::resetGame() {
    for (Character* character : characters) {
        if(character)
        delete character;
    }
    characters.clear();
    map.first();
    map.reset();
    gameHud.resetGame();
    pausedForTransition = false;
    pausedUpdateCharacters = false;
    if(gamePlay == GamePlay::PLAYDEVELOPEDMAP) {
        gameState = new TitleScreenState(this);
    } else {
        gameState = new ChooseCustomizedMapState(this);
    }
}

void World::nextMap() {
    if(map.next()) {
        for (Character* character : characters) {
            character->reset(false);
        }
        gameHud.addHistory();
        setGameState(new PlayingState(this));
    } else {
        setGameState(new FinishedState(this));
    }
}

int World::getMinLives() {
    int minLives = characters[0]->getLives();
    for (Character* character : characters) {
        if (character->getLives() < minLives) {
            minLives = character->getLives();
        }
    }
    return minLives;
}

void World::resetWhenCharacterDead() {
    if(!isPlayerDownMusicStreamPlaying() && !isGameOverMusicStreamPlaying()) {
        if(getMinLives() > 0) {
            resetMap();
        }
        else if (getMinLives() == 0) {
            playGameOverMusic();
            setGameState(new GameOverState(this));
            for (Character* character : characters) {
                character->setLives(-1);
            }
        }
        else {
            resetGame();
        }
    }
}

void World::setIsClosed(bool isClosed) {
    this->isClosed=isClosed;
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
    j["music"] = json::array();
    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    for (const auto& [name, musicStream] : music) {
        bool isPlaying = IsMusicStreamPlaying(musicStream);
        float timePlayed = isPlaying ? GetMusicTimePlayed(musicStream) : 0.0f;
        
        j["music"].push_back({
            {"name", name},
            {"isPlaying", isPlaying},
            {"timePlayed", timePlayed}
        });
    }
    j["gameMode"] = static_cast<int>(gameMode);
    j["gamePlay"] = static_cast<int>(gamePlay);
    return j;
}

void World::loadFromJson(const json& j) {
    characters.clear();
    for (const auto& characterJson : j["characters"]) {
        Character* character = CharacterFactory::createCharacter( static_cast<CharacterName>(characterJson["characterName"].get<int>()), static_cast<ModePlayer>(characterJson["modePlayer"].get<int>()) );
        character->loadFromJson(characterJson);
        character->setWorld(this);
        keyManager.setKeyManagerForCharacter(character, static_cast<ModePlayer>(characterJson["modePlayer"].get<int>()));
        characters.push_back(character);
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

    std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
    for (auto& [name, musicStream] : music) {
        if (IsMusicStreamPlaying(musicStream)) {
            StopMusicStream(musicStream);
        }
    }
    for (int i = 0; i < j["music"].size(); ++i) {
        const auto& musicJson = j["music"][i];
        if (music.find(musicJson["name"]) != music.end()) {
            if (musicJson["isPlaying"]) {
                PlayMusicStream(music[musicJson["name"]]);
                UpdateMusicStream(music[musicJson["name"]]);
                SeekMusicStream(music[musicJson["name"]], musicJson["timePlayed"]);
            }
        }
    }
    
    GameState* gameState = GameStateFactory::createGameState(this, static_cast<GameStateType>(j["gameState"]["gameStateType"].get<int>()));
    if (gameState != nullptr) {
        gameState->loadFromJson(j["gameState"]);
        setGameState(gameState);
    } else {
        throw std::runtime_error("Failed to create game state from JSON");
    }
    gameMode = static_cast<GameMode>(j["gameMode"].get<int>());
    gamePlay = static_cast<GamePlay>(j["gamePlay"].get<int>());
}

void World::setGameMode(GameMode mode) {
    gameMode = mode;
}

GameMode World::getGameMode() const {
    return gameMode;
}

void World::setGamePlay(GamePlay gamePlay) {
    this->gamePlay = gamePlay;
}

GamePlay World::getGamePlay() const {
    return gamePlay;
}