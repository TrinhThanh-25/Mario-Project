#include "GUI/GameHud.h"
#include "Item/Item.h"
#include "Game/World.h"
#include "Common/ResourceManager.h"
#include "GameState/TimeUpState.h"

GameHud::GameHud(World* world, int yoshiCoins, int coins, int lives, int points, float maxTime)
    : yoshiCoins(yoshiCoins), 
    coins(coins), 
    lives(lives), 
    points(points), 
    maxTime(maxTime), 
    ellapsedTime(0.0f),
    powerUpItem (CharacterType::SMALL),
    world(world),
    map(world->getMap()),
    characters(world->getCharacters()) {}

GameHud::~GameHud() {}

void GameHud::update() {
    float deltaTime = GetFrameTime();
    ellapsedTime += deltaTime;
    if (ellapsedTime >= maxTime) {
        world->playPlayerDownMusic();
        lives--;
        for (int i = 0; i < characters.size(); i++) {
            characters[i]->setState(SpriteState::DYING);
            characters[i]->transitionToSmall();
        }
        world->setGameState(new TimeUpState(world));
    }
}

void GameHud::draw() const {
    //
    std::unordered_map<std::string, Texture2D>& textures = ResourceManager::getTexture();

    DrawTexture( textures["GuiMario"], 34, 32, WHITE );
    DrawTexture( textures["GuiX"], 54, 49, WHITE );
    ResourceManager::drawWhiteSmallNumber( lives < 0 ? 0 : lives, 68, 49 );

    for ( int i = 0; i < yoshiCoins; i++ ) {
        DrawTexture( textures["GuiCoin"], 34 + textures["GuiMario"].width + 16 + i*textures["GuiCoin"].width, 32, WHITE);
    }

    if ( powerUpItem == CharacterType::SUPER ) {
        DrawTexture( textures["Mushroom"], GetScreenWidth() / 2 - textures["Mushroom"].width / 2, 32, WHITE );
    } else if ( powerUpItem == CharacterType::FLOWER ) {
        DrawTexture( textures["FireFlower0"], GetScreenWidth() / 2 - textures["FireFlower0"].width / 2, 32, WHITE );
    }
    DrawTexture( textures["GuiNextItem"], GetScreenWidth() / 2 - textures["GuiNextItem"].width / 2, 20, WHITE );

    int time = getRemainingTime();
    time = time < 0 ? 0 : time;

    DrawTexture( textures["GuiTime"], GetScreenWidth() - 34 - 176, 32, WHITE );
    ResourceManager::drawYellowSmallNumber( time, GetScreenWidth() - 34 - 128 - ResourceManager::getSmallNumberWidth( time ), 50 );

    DrawTexture( textures["GuiCoin"], GetScreenWidth() - 115, 32, WHITE );
    DrawTexture( textures["GuiX"], GetScreenWidth() - 97, 34, WHITE );
    ResourceManager::drawWhiteSmallNumber( coins, GetScreenWidth() - 34 - ResourceManager::getSmallNumberWidth( coins ), 34 );
    ResourceManager::drawWhiteSmallNumber( points, GetScreenWidth() - 34 - ResourceManager::getSmallNumberWidth( points ), 50 );
}

void GameHud::reset() {
    yoshiCoins = 0;
    coins = 0;
    lives = 5;
    points = 0;
    ellapsedTime = 0.0f;
}

void GameHud::setLives(int lives) {
    this->lives = lives;
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

int GameHud::getLives() const {
    return lives;
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

void GameHud::addLives(int lives) {
    this->lives += lives;
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

void GameHud::removeLives(int lives) {
    this->lives -= lives;
    if (this->lives < 0) {
        this->lives = 0;
    }
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

void GameHud::setPowerUpItem(CharacterType type) {
    this->powerUpItem = type;
}

CharacterType GameHud::getPowerUpItem() const {
    return powerUpItem;
}

void GameHud::releasePowerUpItem() {
    Item* item = nullptr;

    if(powerUpItem == CharacterType::SUPER) {
        Vector2 position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["Mushroom"].width / 2, 32}, *world->getCamera());
        // item = new mushroom
    } else if(powerUpItem == CharacterType::FLOWER) {
        Vector2 position = GetScreenToWorld2D({(float) GetScreenWidth() / 2 - ResourceManager::getTexture()["FireFlower0"].width / 2, 32}, *world->getCamera());
        // item = new flower
    }
    powerUpItem = CharacterType::SMALL;   

    if(item) {
        item->setState(SpriteState::ACTIVE);
        item->setDirection(Direction::RIGHT);
        map->getItem().push_back(item);
        PlaySound(ResourceManager::getSound()["ReleasePowerUpItem"]);
    }
}

void GameHud::reset(bool isPowerOff) {
    if (isPowerOff) {
        powerUpItem = CharacterType::SMALL;
    }
    yoshiCoins = 0;
    ellapsedTime = 0.0f;
}

void GameHud::resetGame() {
    reset(true);
    lives = 5;
    coins = 0;
    points = 0;
}

json GameHud::saveToJson() const {
    json j;
    j["lives"] = lives;
    j["coins"] = coins;
    j["yoshiCoins"] = yoshiCoins;
    j["points"] = points;
    j["maxTime"] = maxTime;
    j["ellapsedTime"] = ellapsedTime;
    j["powerUpItem"] = static_cast<int>(powerUpItem);
    return j;
}

void GameHud::loadFromJson(const json& j) {
    lives = j.value("lives", 5);
    coins = j.value("coins", 0);
    yoshiCoins = j.value("yoshiCoins", 0);
    points = j.value("points", 0);
    maxTime = j.value("maxTime", 200.0f);
    ellapsedTime = j.value("ellapsedTime", 0.0f);
    powerUpItem = static_cast<CharacterType>(j.value("powerUpItem", static_cast<int>(CharacterType::SMALL)));
}