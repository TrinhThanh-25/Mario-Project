#ifndef WORLD_H
#define WORLD_H

#include "Character/Mario.h"
#include "Character/Luigi.h"
#include "Common/Drawable.h"
#include "Game/Map.h"
#include "Game/GameLoop.h"
#include "GUI/GameHud.h"
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;

enum ModeWorld {
    SINGLEPLAYER,
    MULTIPLAYER,
};

class Character;
class GameState;

class World : public GameLoop {
    private:
        std::vector<Character*> characters;
        Map map;
        Camera2D camera;
        GameHud gameHud;
        
        int width;
        int height;
        std::string title;
        int FPS;

        int remainTimePoint;

        GameState* gameState;
        ModeWorld modeWorld;
        
        bool playerDownMusicStreamPlaying;
        bool gameOverMusicStreamPlaying;
        bool pausedForTransition;
        bool pausedUpdateCharacters;
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

        void setModeWorld(ModeWorld mode);
        ModeWorld* getModeWorld();

        std::vector<Character*>& getCharacters();
        Map* getMap();
        int* getRemainTimePoint();
        GameHud* getGameHud();

        void playPlayerDownMusic();
        void playGameOverMusic();
        bool isPlayerDownMusicStreamPlaying() const;
        bool isGameOverMusicStreamPlaying() const;

        void pauseWorld(bool pausedForTransition, bool pausedUpdateCharacters);
        void resumeWorld();
        bool* getPausedForTransition();
        bool* getPausedUpdateCharacters();

        void resetMap();
        void resetGame();
        void newGame();
        void nextMap();
        void resetWhenCharacterDead();

        json saveToJson() const;
        void loadFromJson(const json& j);
};

#endif