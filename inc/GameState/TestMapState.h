#ifndef TESTMAPSTATE_H
#define TESTMAPSTATE_H

#include "GameState/GameState.h"
#include "Game/World.h"
#include "Character/CharacterName.h"

class TestMapState : public GameState {
    private:
        Map* map;
        Camera2D* camera;
        std::vector<Character*>& characters;
        bool* pausedForTransition;
        bool* pausedUpdateCharacters;
        bool creative = false;
        bool immortal = false;
        bool net = false;
        
        int currentCharacterIndex = 0;
        std::vector<CharacterName> availableCharacters = {
            CharacterName::MARIO,
            CharacterName::LUIGI,
            CharacterName::TOAD,
            CharacterName::PEACH
        };
        std::vector<std::string> characterNames = {
            "Mario",
            "Luigi", 
            "Toad",
            "Peach"
        };
        
        void changeCharacter(CharacterName newCharacter);
    public:
        TestMapState(World* world, std::string mapFileName);
        ~TestMapState() override;
        void update() override;
        void draw() override;
};

#endif