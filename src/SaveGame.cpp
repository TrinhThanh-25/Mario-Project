#include "SaveGame.h"
#include "Game/World.h"
#include "Character/Character.h"
#include "Character/Fireball.h"
#include "GameState/GameState.h"
#include "GameState/CountingPointState.h"
#include "GameState/GoNextMapState.h"
#include "GameState/IrisOutState.h"
#include "GameState/PlayingState.h"
#include "GameState/TimeUpState.h"
#include <fstream>
#include <stdexcept>
#include <filesystem>

void SaveGame::saveGame(const World& world, const std::string& filename) {
    json j;
    j["characters"] = json::array();
    for (const auto& character : world.getCharacters()) {
        json charJson;
        charJson["name"] = character->getName();
        charJson["modePlayer"] = character->getModePlayer();
        charJson["position"] = {character->getPosition().x, character->getPosition().y};
        charJson["size"] = {character->getSize().x, character->getSize().y};
        charJson["velocity"] = {character->getVelocity().x, character->getVelocity().y};
        charJson["speed"] = character->getSpeed();
        charJson["maxSpeed"] = character->getMaxSpeed();
        charJson["jumpSpeed"] = character->getJumpSpeed();
        charJson["isRunning"] = character->getIsRunning();
        charJson["isDucking"] = character->getIsDucking();
        charJson["walkingAcum"] = character->getWalkingAcum();
        charJson["walkingBeforeRunningAcum"] = character->getWalkingBeforeRunningAcum();
        charJson["drawRunning"] = character->getDrawRunning();
        charJson["invulnerable"] = character->getInvulnerable();
        charJson["invulnerableAcum"] = character->getInvulnerableAcum();
        charJson["invulnerableBlink"] = character->getInvulnerableBlink();
        charJson["invincible"] = character->getInvincible();
        charJson["invincibleAcum"] = character->getInvincibleAcum();
        charJson["transitionAcum"] = character->getTransitionAcum();
        charJson["transitionCurrentFrame"] = character->getTransitionCurrentFrame();
        charJson["transitionCurrentIndex"] = character->getTransitionCurrentIndex();
        charJson["oldPosition"] = {character->getOldPosition().x, character->getOldPosition().y};
        charJson["type"] = character->getType();

        charJson["fireball"] = json::array();
        for (const auto& fb : character->getFireballs()) {
            json fbJson;
            fbJson["position"] = {fb.getPosition().x, fb.getPosition().y};
            fbJson["direction"] = fb.getDirection();
            fbJson["fireAcum"] = fb.getFireAcum();
            fbJson["fireTime"] = fb.getFireTime();
            fbJson["frameAcum"] = fb.getFrameAcum();
            fbJson["curFrame"] = fb.getCurFrame();
            fbJson["state"] = fb.getState();
            charJson["fireball"].push_back(fbJson);
        }

        charJson["previousState"] = character->getPreviousState();
        charJson["state"] = character->getState();
        charJson["direction"] = character->getDirection();
        charJson["frameAcum"] = character->getFrameAcum();
        charJson["curFrame"] = character->getCurFrame();
        j["Characters"].push_back(charJson);
    }

    Map* map = world.getMap();
    j["map"] = {
        {"offset", map->getOffset()},
        {"mapNumber", map->getMapNumber()},
        {"width", map->getWidth()},
        {"height", map->getHeight()},
        {"backgroundColor", {
            {"r", map->getBackgroundColor().r},
            {"g", map->getBackgroundColor().g},
            {"b", map->getBackgroundColor().b},
            {"a", map->getBackgroundColor().a}
        }},
        {"tile", json::array()},
        {"backTile", json::array()},
        {"frontTile", json::array()},
        {"block", json::array()},
        {"messBlock", json::array()},
        {"backEnemy", json::array()},
        {"frontEnemy", json::array()},
        {"item", json::array()},
        {"staticItem", json::array()}
    };
    
    for (const auto& tile : map->getTiles()) {
        json tileJson;
        tileJson["position"] = {tile->getPosition().x, tile->getPosition().y};
        tileJson["size"] = {tile->getSize().x, tile->getSize().y};
        tileJson["nameTexture"] = tile->getNameTexture();
        tileJson["type"] = tile->getType();
        j["map"]["tile"].push_back(tileJson);
    }

    for (const auto& backTile : map->getBackTiles()) {
        json tileJson;
        tileJson["position"] = {backTile->getPosition().x, backTile->getPosition().y};
        tileJson["size"] = {backTile->getSize().x, backTile->getSize().y};
        tileJson["nameTexture"] = backTile->getNameTexture();
        tileJson["type"] = backTile->getType();
        j["map"]["backTile"].push_back(tileJson);
    }

    for (const auto& frontTile : map->getFrontTiles()) {
        json tileJson;
        tileJson["position"] = {frontTile->getPosition().x, frontTile->getPosition().y};
        tileJson["size"] = {frontTile->getSize().x, frontTile->getSize().y};
        tileJson["nameTexture"] = frontTile->getNameTexture();
        tileJson["type"] = frontTile->getType();
        j["map"]["frontTile"].push_back(tileJson);
    }
    // block, enemy

    Camera2D* camera = world.getCamera();
    j["camera"] = {
        {"offset", {camera->offset.x, camera->offset.y}},
        {"target", {camera->target.x, camera->target.y}},
        {"rotation", camera->rotation},
        {"zoom", camera->zoom}
    };

    GameHud* gameHud = world.getGameHud();
    j["gameHud"] = {
        {"lives", gameHud->getLives()},
        {"coins", gameHud->getCoins()},
        {"yoshiCoins", gameHud->getYoshiCoins()},
        {"points", gameHud->getPoints()},
        {"maxTime", gameHud->getMaxTime()},
        {"ellapsedTime", gameHud->getEllapsedTime()},
        {"powerUpItem", gameHud->getPowerUpItem()}
    };
    
    j["remainTimePoint"] = world.getRemainTimePoint();

    GameState* gameState = world.getGameState();
    j["gameState"]["gameStateType"] = gameState->getGameStateType();
    switch(gameState->getGameStateType()) {
        case GameStateType::COUNTING_POINT:
            j["gameState"]["isGetRemainTimePoint"] = static_cast<CountingPointState*>(gameState)->getIsGetRemainTimePoint();
            break;
        case GameStateType::GO_NEXT_MAP:
            j["gameState"]["irisOutAcum"] = static_cast<GoNextMapState*>(gameState)->getIrisOutAcum();
            j["gameState"]["isIrisOutFinished"] = static_cast<GoNextMapState*>(gameState)->getIsIrisOutFinished();
            break;
        case GameStateType::IRIS_OUT:
            break;
        case GameStateType::PLAYING:
            break;
        case GameStateType::TIME_UP:
            break;
    }
    
    j["modeWorld"] = world.getModeWorld();
    j["playerDownMusicStreamPlaying"] = world.isPlayerDownMusicStreamPlaying();
    j["gameOverMusicStreamPlaying"] = world.isGameOverMusicStreamPlaying();
    j["pausedForTransition"] = *world.getPausedForTransition();
    j["pausedUpdateCharacters"] = *world.getPausedUpdateCharacters();
    std::ofstream out(filename);
    if(out.is_open()) {
        out << j.dump(4);
        out.close();
    } else {
        throw std::runtime_error("Could not open file for saving game: " + filename);
    }
}

void SaveGame::loadGame(World& world, const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        throw std::runtime_error("Save game file does not exist: " + filename);
    }
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open save game file: " + filename);
    }
    json j;
    in >> j;
    in.close();
    world.resetGame();
    world.getCharacters().clear();
    for (const auto& charJson : j["Characters"]) {
        Character* character = nullptr;
        if (charJson["name"] == "Mario") {
            character = new Mario(
                charJson["modePlayer"].get<ModePlayer>(),
                Vector2{charJson["position"][0], charJson["position"][1]},
                Vector2{charJson["size"][0], charJson["size"][1]},
                RED,
                charJson["speed"],
                charJson["maxSpeed"],
                charJson["jumpSpeed"]
            );
        } else if (charJson["name"] == "Luigi") {
            character = new Luigi(
                charJson["modePlayer"].get<ModePlayer>(),
                Vector2{charJson["position"][0], charJson["position"][1]},
                Vector2{charJson["size"][0], charJson["size"][1]},
                GREEN,
                charJson["speed"],
                charJson["maxSpeed"],
                charJson["jumpSpeed"]
            );
        }
        character->setIsRunning(charJson["isRunning"]);
        character->setIsDucking(charJson["isDucking"]);
        character->setWalkingAcum(charJson["walkingAcum"]);
        character->setWalkingBeforeRunningAcum(charJson["walkingBeforeRunningAcum"]);
        character->setDrawRunning(charJson["drawRunning"]);
        character->setInvulnerable(charJson["invulnerable"]);
        character->setInvulnerableAcum(charJson["invulnerableAcum"]);
        character->setInvulnerableBlink(charJson["invulnerableBlink"]);
        character->setInvincible(charJson["invincible"]);
        character->setInvincibleAcum(charJson["invincibleAcum"]);
        character->setTransitionAcum(charJson["transitionAcum"]);
        character->setTransitionCurrentFrame(charJson["transitionCurrentFrame"]);
        character->setTransitionCurrentIndex(charJson["transitionCurrentIndex"]);
        character->setOldPosition(Vector2{charJson["oldPosition"][0], charJson["oldPosition"][1]});
        character->setType(static_cast<CharacterType>(charJson["type"]));

        for (const auto& fb : charJson["fireball"]) {
            Fireball fireball(
                Vector2{fb["position"][0], fb["position"][1]},
                static_cast<Direction>(fb["direction"]),
                fb["fireTime"]
            );
            fireball.setFireAcum(fb["fireAcum"]);
            fireball.setFrameAcum(fb["frameAcum"]);
            fireball.setCurFrame(fb["curFrame"]);
            fireball.setState(fb["state"]);
            character->addFireball(fireball);
        }

        character->setPreviousState(static_cast<SpriteState>(charJson["previousState"]));
        character->setState(static_cast<SpriteState>(charJson["state"]));
        character->setDirection(static_cast<Direction>(charJson["direction"]));
        character->setFrameAcum(charJson["frameAcum"]);
        character->setCurFrame(charJson["curFrame"]);
        world.addCharacter(character);
    }

    Map* map = world.getMap();
    map->setOffset(j["map"]["offset"]);
    map->setMapNumber(j["map"]["mapNumber"]);
    map->setWidth(j["map"]["width"]);
    map->setHeight(j["map"]["height"]);
    map->setBackgroundColor(Color{
        j["map"]["backgroundColor"]["r"],
        j["map"]["backgroundColor"]["g"],
        j["map"]["backgroundColor"]["b"],
        j["map"]["backgroundColor"]["a"]
    });
    map->clear();
    for (const auto& tileJson : j["map"]["tile"]) {
        Tile* tile = new Tile(
            Vector2{tileJson["position"][0], tileJson["position"][1]},
            Vector2{tileJson["size"][0], tileJson["size"][1]},
            tileJson["nameTexture"],
            static_cast<TileType>(tileJson["type"])
        );
        map->getTile().push_back(tile);
    }
    for (const auto& backTileJson : j["map"]["backTile"]) {
        Tile* backTile = new Tile(
            Vector2{backTileJson["position"][0], backTileJson["position"][1]},
            Vector2{backTileJson["size"][0], backTileJson["size"][1]},
            backTileJson["nameTexture"],
            static_cast<TileType>(backTileJson["type"])
        );
        map->getBackTile().push_back(backTile);
    }
    for (const auto& frontTileJson : j["map"]["frontTile"]) {
        Tile* frontTile = new Tile(
            Vector2{frontTileJson["position"][0], frontTileJson["position"][1]},
            Vector2{frontTileJson["size"][0], frontTileJson["size"][1]},
            frontTileJson["nameTexture"],
            static_cast<TileType>(frontTileJson["type"])
        );
        map->getFrontTile().push_back(frontTile);
    }

    // block, enemy

    Camera2D* camera = world.getCamera();
    camera->offset = Vector2{j["camera"]["offset"][0], j["camera"]["offset"][1]};
    camera->target = Vector2{j["camera"]["target"][0], j["camera"]["target"][1]};
    camera->rotation = j["camera"]["rotation"];
    camera->zoom = j["camera"]["zoom"];

    GameHud* gameHud = world.getGameHud();
    gameHud->setLives(j["gameHud"]["lives"]);
    gameHud->setCoins(j["gameHud"]["coins"]);
    gameHud->setYoshiCoins(j["gameHud"]["yoshiCoins"]);
    gameHud->setPoints(j["gameHud"]["points"]);
    gameHud->setMaxTime(j["gameHud"]["maxTime"]);
    gameHud->setEllapsedTime(j["gameHud"]["ellapsedTime"]);
    gameHud->setPowerUpItem(static_cast<CharacterType>(j["gameHud"]["powerUpItem"]));

    world.setRemainTimePoint(j["remainTimePoint"]);

    switch(j["gameState"]["gameStateType"].get<GameStateType>()) {
        case GameStateType::COUNTING_POINT: {
            CountingPointState* countingPointState = new CountingPointState(&world);
            countingPointState->setGetRemainTimePoint(j["gameState"]["isGetRemainTimePoint"]);
            world.setGameState(countingPointState);
            break;
        }
        case GameStateType::GO_NEXT_MAP: {
            GoNextMapState* goNextMapState = new GoNextMapState(&world);
            goNextMapState->setIrisOutAcum(j["gameState"]["irisOutAcum"]);
            goNextMapState->setIsIrisOutFinished(j["gameState"]["isIrisOutFinished"]);
            world.setGameState(goNextMapState);
            break;
        }
        case GameStateType::IRIS_OUT:
            world.setGameState(new IrisOutState());
            break;
        case GameStateType::PLAYING:
            world.setGameState(new PlayingState());
            break;
        case GameStateType::TIME_UP:
            world.setGameState(new TimeUpState());
            break;
    }

    world.setModeWorld(static_cast<ModeWorld>(j["modeWorld"]));
    world.setPlayerDownMusicStreamPlaying(j["playerDownMusicStreamPlaying"]);
    world.setGameOverMusicStreamPlaying(j["gameOverMusicStreamPlaying"]);
    world.pauseWorld(j["pausedForTransition"], j["pausedUpdateCharacters"]);
}

void SaveGame::deleteSaveGame(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

bool SaveGame::saveGameExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}