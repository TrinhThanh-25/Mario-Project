#ifndef IRISOUTSTATE_H
#define IRISOUTSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class IrisOutState : public GameState {
    private:
        GameHud* gameHud;
        Map* map;
        Camera2D* camera;
    public:
        IrisOutState(World* world);
        ~IrisOutState() override;
        void update() override;
        void draw() override;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif