#ifndef GAMEHUDMEMENTO_H
#define GAMEHUDMEMENTO_H

class GameHudMemento {
    private:
        int coins;
        int yoshiCoins;
        int points;
    public:
        GameHudMemento(int coins, int yoshiCoins, int points);
        int getCoins() const;
        int getYoshiCoins() const;
        int getPoints() const;
};

#endif