#ifndef PLAYINGSTATE_H
#define PLAYINGSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"

class PlayingState : public GameState {
    private:
        std::vector<Character*>& characters;
        Map* map;
        Camera2D* camera;
        ModeWorld* modeWorld;
        GameHud* gameHud;
        bool* pausedForTransition;
        bool* pausedUpdateCharacters;
    public:
        PlayingState(World* world);
        ~PlayingState() override;
        void update() override;
        void draw() override;
        bool isOneCharactersDead() const;
        bool isAllCharactersVictory() const;

        json saveToJson() const override;
        void loadFromJson(const json& j) override;
};

#endif