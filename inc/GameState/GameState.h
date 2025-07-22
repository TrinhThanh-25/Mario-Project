#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "json.hpp"
#include "GameState/GameStateType.h"

using json = nlohmann::json;

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