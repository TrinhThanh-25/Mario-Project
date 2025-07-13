#include "Game/GameHud.h"
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
    // Drawing logic for the HUD would go here
    // This could include drawing lives, coins, yoshi coins, points, and remaining time
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
    // Item initial position

    if(powerUpItem == CharacterType::SUPER) {
        // item = new mushroom
    } else if(powerUpItem == CharacterType::FLOWER) {
        // item = new flower
    }

    if(item) {
        item->setState(SpriteState::ACTIVE);
        item->setDirection(Direction::RIGHT);
        map->getItem().push_back(item);
        PlaySound(ResourceManager::getSound()["ReleasePowerUpItem"]);
    }
    powerUpItem = CharacterType::SMALL;   
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
    maxTime = 200.0f;
}