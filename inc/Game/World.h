#ifndef WORLD_H
#define WORLD_H

#include "Character/Character.h"
#include "Character/Mario.h"
#include "Character/Luigi.h"
#include "Common/Drawable.h"
#include "Game/Map.h"
#include "GameState/GameState.h"
#include "Game/GameLoop.h"
#include <vector>
#include <string>

enum ModeWorld {
    SINGLEPLAYER,
    MULTIPLAYER,
};

class World : public GameLoop {
    private:
        std::vector<Character*> characters;
        Map map;
        Camera2D camera;
        
        //Window
        int width;
        int height;
        std::string title;
        int FPS;

        GameState* gameState;
        ModeWorld modeWorld;
    public:
        static float gravity;
    public:
        World(int width, int height, const std::string& title, int FPS);
        ~World() override;

        void init() override;
        void update() override;
        void draw() override;
        void updateCamera() override;
        
        void setGameState(GameState* newState);

        void setCamera(Camera2D *camera);
        Camera2D* getCamera();

        std::vector<Character*>& getCharacters();
        Map* getMap();
        ModeWorld* getModeWorld();

};

#endif