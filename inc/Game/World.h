#ifndef WORLD_H
#define WORLD_H

#include "Common/Drawable.h"
#include "Game/Map.h"
#include "Game/GameLoop.h"
#include "GUI/GameHud.h"
#include <vector>
#include <string>
#include <memory>
#include "json.hpp"
#include "Game/ModeWorld.h"
#include "Game/GameMode.h"
#include "Game/GamePlay.h"
#include "Common/KeyManager.h"
#include "Common/GamepadManager.h"

using json = nlohmann::json;

class Character;
class GameState;
enum class ModePlayer;

class World : public GameLoop {
    private:
        std::vector<Character*> characters;
        Map map;
        Camera2D camera;
        GameHud gameHud;
        KeyManager keyManager;
        GamepadManager gamepadManager;
        
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

        GameMode gameMode;
        GamePlay gamePlay;
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
        KeyManager* getKeyManager();
        GamepadManager* getGamepadManager();

        void playPlayerDownMusic();
        void playGameOverMusic();
        void stopPlayerDownMusic();
        void stopGameOverMusic();
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
        int getMinLives();
        void resetWhenCharacterDead();

        void setIsClosed(bool isClosed);

        json saveToJson() const;
        void loadFromJson(const json& j);

        void setGameMode(GameMode mode);
        GameMode getGameMode() const;
        void setGamePlay(GamePlay gamePlay);
        GamePlay getGamePlay() const;
};

#endif