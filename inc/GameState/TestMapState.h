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
        float offsetX = 0.0f;
        float offsetY = 0.0f;
        
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
        int currentTypeIndex = 0;
        std::vector<CharacterType> availableTypes = {
            CharacterType::SMALL,
            CharacterType::SUPER,
            CharacterType::FLOWER
        };
        std::vector<std::string> typeNames = {
            "Small",
            "Super",
            "Flower"
        };

        std::string mapFileName;
        bool isSaved = false;
        
        void changeCharacter(CharacterName newCharacter);
    public:
        TestMapState(World* world);
        TestMapState(World* world, std::string mapFileName);
        TestMapState(World* world, std::string mapFileName, int width, int height, const std::vector<int>& mapGrid);
        ~TestMapState() override;
        
        void setMapFileName(const std::string& mapFileName);
        void update() override;
        void draw() override;
        void setMap(int width, int height, const std::vector<int>& mapGrid);
        void setIsSaved(bool saved);
};

#endif