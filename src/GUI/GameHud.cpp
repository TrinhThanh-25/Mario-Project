#include "GUI/GameHud.h"
#include "Item/Item.h"
#include "Game/World.h"
#include "Common/ResourceManager.h"
#include "GameState/TimeUpState.h"
#include "GameState/GameStateFactory.h"
#include "Item/ItemFactory.h"

GameHud* GameHud::gameHud = nullptr;

GameHudHistory::GameHudHistory()
    : currentIndex(-1) {}

void GameHudHistory::addMemento(const GameHudMemento& memento) {
    history.clear();
    history.push_back(memento);
    currentIndex = 0;
}

GameHudMemento GameHudHistory::getMemento() const {
    if (currentIndex >= 0 && currentIndex < history.size()) {
        return history[currentIndex];
    }
    throw std::out_of_range("No memento available at current index.");
}

void GameHudHistory::clear() {
    history.clear();
    currentIndex = -1;
}

json GameHudHistory::saveToJson() const {
    json j;
    j["history"] = json::array();
    for (const auto& memento : history) {
        j["history"].push_back({
            {"coins", memento.getCoins()},
            {"yoshiCoins", memento.getYoshiCoins()},
            {"points", memento.getPoints()}
        });
    }
    return j;
}

void GameHudHistory::loadFromJson(const json& j) {
    if (j.contains("history")) {
        history.clear();
        for (const auto& memento : j["history"]) {
            history.push_back(GameHudMemento(
                memento["coins"].get<int>(),
                memento["yoshiCoins"].get<int>(),
                memento["points"].get<int>()
            ));
        }
        currentIndex = history.empty() ? -1 : 0;
    }
}

GameHud* GameHud::getInstance(World* world, int yoshiCoins, int coins, int points, float maxTime) {
    if (!gameHud) {
        gameHud = new GameHud(world, yoshiCoins, coins, points, maxTime);
    }
    return gameHud;
}

GameHud::GameHud(World* world, int yoshiCoins, int coins, int points, float maxTime)
    : yoshiCoins(yoshiCoins), 
    coins(coins),
    points(points), 
    maxTime(maxTime), 
    ellapsedTime(0.0f),
    world(world),
    map(world->getMap()),
    characters(world->getCharacters()) {
        history.addMemento(GameHudMemento(coins, yoshiCoins, points));
}

GameHud::~GameHud() {}

void GameHud::update() {
    float deltaTime = GetFrameTime();
    ellapsedTime += deltaTime;
    if (ellapsedTime >= maxTime) {
        world->playPlayerDownMusic();
        for (int i = 0; i < characters.size(); i++) {
            characters[i]->setState(SpriteState::DYING);
            characters[i]->transitionToSmall();
            characters[i]->setLives(characters[i]->getLives() - 1);
        }
        world->setGameState(GameStateFactory::createGameState(world, GameStateType::TIME_UP));
    }
}

void GameHud::draw() const {
    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();

    for ( int i = 0; i < yoshiCoins; i++ ) {
        DrawTexture( textures["GuiCoin"], 34 + textures["GuiMario"].width + 16 + i*textures["GuiCoin"].width, 32, WHITE);
    }

    int time = getRemainingTime();
    time = time < 0 ? 0 : time;

    DrawTexture( textures["GuiTime"], GetScreenWidth() - 34 - 176, 32, WHITE );
    ResourceManager::drawYellowSmallNumber( time, GetScreenWidth() - 34 - 128 - ResourceManager::getSmallNumberWidth( time ), 50 );

    DrawTexture( textures["GuiCoin"], GetScreenWidth() - 115, 32, WHITE );
    DrawTexture( textures["GuiX"], GetScreenWidth() - 97, 34, WHITE );
    ResourceManager::drawWhiteSmallNumber( coins, GetScreenWidth() - 34 - ResourceManager::getSmallNumberWidth( coins ), 34 );
    ResourceManager::drawWhiteSmallNumber( points, GetScreenWidth() - 34 - ResourceManager::getSmallNumberWidth( points ), 50 );
    
    for (Character* character : characters) {
        character->drawGameHud();
    }
}

void GameHud::reset() {
    coins = history.getMemento().getCoins();
    yoshiCoins = history.getMemento().getYoshiCoins();
    points = history.getMemento().getPoints();
    ellapsedTime = 0.0f;
}

void GameHud::resetGame() {
    coins = 0;
    yoshiCoins = 0;
    points = 0;
    ellapsedTime = 0.0f;
    history.clear();
    history.addMemento(GameHudMemento(coins, yoshiCoins, points));
}

void GameHud::addHistory() {
    history.addMemento(GameHudMemento(coins, yoshiCoins, points));
}

void GameHud::setCoins(int coins) {
    this->coins = coins;
}

void GameHud::setYoshiCoins(int yoshiCoins) {
    this->yoshiCoins = yoshiCoins;
}

void GameHud::setPoints(int points) {
    this->points = points;
}

void GameHud::setMaxTime(float maxTime) {
    this->maxTime = maxTime;
}

void GameHud::setEllapsedTime(float time) {
    this->ellapsedTime = time;
}

int GameHud::getCoins() const {
    return coins;
}

int GameHud::getYoshiCoins() const {
    return yoshiCoins;
}

int GameHud::getPoints() const {
    return points;
}

int GameHud::getRemainingTime() const {
    return static_cast<int>(maxTime - ellapsedTime);
}

void GameHud::addCoins(int coins) {
    this->coins += coins;
}

void GameHud::addYoshiCoins(int yoshiCoins) {
    this->yoshiCoins += yoshiCoins;
}

void GameHud::addPoints(int points) {
    this->points += points;
}

void GameHud::removeCoins(int coins) {
    this->coins -= coins;
    if (this->coins < 0) {
        this->coins = 0;
    }
}

void GameHud::removeYoshiCoins(int yoshiCoins) {
    this->yoshiCoins -= yoshiCoins;
    if (this->yoshiCoins < 0) {
        this->yoshiCoins = 0;
    }
}

void GameHud::removePoints(int points) {
    this->points -= points;
    if (this->points < 0) {
        this->points = 0;
    }
}

json GameHud::saveToJson() const {
    json j;
    j["history"] = history.saveToJson();
    j["coins"] = coins;
    j["yoshiCoins"] = yoshiCoins;
    j["points"] = points;
    j["maxTime"] = maxTime;
    j["ellapsedTime"] = ellapsedTime;
    return j;
}

void GameHud::loadFromJson(const json& j) {
    if (j.contains("history")) {
        history.loadFromJson(j["history"]);
    }
    coins = j["coins"].get<int>();
    yoshiCoins = j["yoshiCoins"].get<int>();
    points = j["points"].get<int>();
    maxTime = j["maxTime"].get<float>();
    ellapsedTime = j["ellapsedTime"].get<float>();
}