#include "Common/GameHudMemento.h"

GameHudMemento::GameHudMemento(int coins, int yoshiCoins, int points)
    : coins(coins), yoshiCoins(yoshiCoins), points(points) {}

int GameHudMemento::getCoins() const {
    return coins;
}

int GameHudMemento::getYoshiCoins() const {
    return yoshiCoins;
}

int GameHudMemento::getPoints() const {
    return points;
}