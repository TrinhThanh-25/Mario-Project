#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "json.hpp"
#include "GameState/GameStateType.h"
#include "Game/ModeWorld.h"
#include <vector>
#include <string>

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
        
        // Common state management functions
        virtual void setMapFileName(const std::string& mapFileName);
        virtual void setMap(int width, int height, const std::vector<int>& mapGrid);
        virtual void setIsSaved(bool saved);
        virtual void setModeWorld(ModeWorld mode);
        virtual void setStateBeforeSetting(GameStateType state);
        
        // Common map operations
        virtual void loadMap();
        virtual void saveMap();
        virtual void clearMap();
        virtual void applyMapSize();
        
        // Common file operations
        virtual void load(const std::string& fileName = "");
        virtual void save(const std::string& fileName = "");
        
        // Common utility functions
        virtual void handleUnsavedWarning();
        virtual void loadTileTextures();
        virtual void createDefaultMapFile(const std::string& mapName);

        virtual void saveHistory();
        virtual void setCameraPosition(float x, float y);
};

#endif