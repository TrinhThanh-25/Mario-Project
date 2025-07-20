#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "json.hpp"

using json = nlohmann::json;

enum GameStateType {
    CHOOSE_CHARACTER,
    COUNTING_POINT,
    FINISHED,
    GAME_OVER,
    GO_NEXT_MAP,
    IRIS_OUT,
    PLAYING,
    SETTING,
    TIME_UP,
    TITLE_SCREEN,
};

class World;

class GameState {
    protected:
        World* world;
        GameStateType gameStateType;
        GameState(World* world, GameStateType type);
    public:
        virtual void update() = 0;
        virtual void draw() = 0;
        virtual void enter();
        virtual void exit();
        virtual ~GameState() = default;
        virtual GameStateType getGameStateType() const;

        virtual json saveToJson() const;
        virtual void loadFromJson(const json& j);
};

#endif