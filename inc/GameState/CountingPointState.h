#ifndef COUNTINGPOINTSTATE_H
#define COUNTINGPOINTSTATE_H

#include "GameState/GameState.h"

class GameHud;
class Map;
class Camera2D;

class CountingPointState : public GameState {
    private:
        int* remainTimePoint;
        GameHud* gameHud;
        Map* map;
        Camera2D* camera;
        bool isGetRemainTimePoint = false;
    public:
        CountingPointState(World* world);
        ~CountingPointState() override;
        void update() override;
        void draw() override;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif