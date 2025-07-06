#ifndef CHARACTER_H
#define CHARACTER_H

#include "Character/Fireball.h"
#include "Common/Sprite.h"
#include "Common/Direction.h"
#include "Common/SpriteState.h"
#include "Common/CollisionType.h"
#include "Common/CollisionBox.h"
#include "Character/CharacterType.h"
#include "raylib.h"
#include <vector>
#include <string>

enum ModePlayer{
    ONEPLAYER,
    FIRSTPLAYER,
    SECONDPLAYER
};

class World;
class Map;
class Block;
class Tile;
class Enemy;
class GameHud;

class Character : public Sprite {
    protected:
        World* world;
        Map* map;
        GameHud* gameHud;
        std::string name;
        ModePlayer modePlayer;

        //physics movement
        float speed;
        float maxSpeed;
        float jumpSpeed;
        float dyingSpeed;
        bool isRunning;
        bool isDucking;
        bool isLookingUp;

        //walking
        float frameTimeWalking;
        float walkingAcum;

        //running
        float frameTimeRunning;
        float walkingBeforeRunningTime;
        float walkingBeforeRunningAcum;
        bool drawRunning;

        //invulnerability
        bool invulnerable;
        float invulnerableTime;
        float invulnerableAcum;
        bool invulnerableBlink;

        //invincibility
        bool invincible;
        float invincibleTime;
        float invincibleAcum;

        //Transition
        float transitionTime;
        float transitionAcum;

        int normalTransitionSteps;
        int superToFlowerTransitionSteps;

        int transitionCurrentFrame;
        int transitionCurrentIndex;
        const int normalTransitionFrame[11] = { 0, 1, 0, 1, 0, 1, 2, 1, 2, 1, 2 };
        const int reverseTransitionFrame[11] = { 2, 1, 2, 1, 2, 1, 0, 1, 0, 1, 0 };
        const int superToFlowerTransitionFrame[8] = { 0, 1, 0, 1, 0, 1, 0, 1 };

        Vector2 oldPosition;
        CharacterType type;
        std::vector<Fireball> fireball;

        float activateWidth;

        // bool playerDownMusicStreamPlaying;
        // bool gameOverMusicStreamPlaying;
        SpriteState previousState;
    public:
        Character(std::string name, ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        virtual ~Character() override;

        void setWorld(World* world);

        void update() override;
        void draw() override;
        void updateCollisionBoxes() override;

        bool transition(float deltaTime);
        void movement(float deltaTime);

        CollisionType checkCollision(Sprite* sprite) override;
        CollisionType checkCollisionEnemy(Sprite* sprite);
        void collisionTile(Tile* tile);
        void collisionBlock(Block* block);
        void collisionEnemy(Enemy* enemy);

        void transitionToSmall();
        void transitionToSuper();
        void transitionToFlower();

        void setInvulnerable(bool invulnerable);
        bool isInvulnerable() const;
        void setInvincible(bool invincible);
        bool isInvincible() const;

        void setActivateWidth(float width);
        float getActivateWidth() const;

        void setType(CharacterType type);
        CharacterType getType() const;

        void setPreviousState(SpriteState state);
        SpriteState getPreviousState() const;

        void reset(bool isPowerOff);
        void resetGame();
};

#endif