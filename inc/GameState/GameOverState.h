#ifndef GAMEOVERSTATE_H
#define GAMEOVERSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class GameOverState : public GameState {
    private:
    public:
        GameOverState(World* world);
        ~GameOverState() override;
        void update() override;
        void draw() override;
};

#endif