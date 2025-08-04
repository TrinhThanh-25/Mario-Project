#ifndef GAMEHUD_H
#define GAMEHUD_H

#include "Character/CharacterType.h"
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

class Map;
class World;
class Character;

class GameHud {
    private:
        int coins;
        int yoshiCoins;
        int points;
        float maxTime;
        float ellapsedTime;
        World* world;
        Map* map;
        std::vector<Character*>& characters;
    public:
        GameHud(World* world, int yoshiCoins = 0, int coins = 0, int points = 0, float maxTime = 200.0f);
        ~GameHud();

        void update();
        void draw() const;
        void reset();
        
        void setCoins(int coins);
        void setYoshiCoins(int yoshiCoins);
        void setPoints(int points);
        void setMaxTime(float maxTime);
        void setEllapsedTime(float time);

        int getCoins() const;
        int getYoshiCoins() const;
        int getPoints() const;
        int getRemainingTime() const;

        void addCoins(int coins);
        void addYoshiCoins(int yoshiCoins);
        void addPoints(int points);

        void removeCoins(int coins);
        void removeYoshiCoins(int yoshiCoins);
        void removePoints(int points);

        json saveToJson() const;
        void loadFromJson(const json& j);
};

#endif