#ifndef CHARACTER_H
#define CHARACTER_H

#include "Character/Fireball.h"
#include "Character/CharacterType.h"
#include "Character/CharacterName.h"
#include "Game/GameMode.h"
#include "raylib.h"
#include <vector>
#include <string>

enum class ModePlayer{
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
        CharacterName characterName;
        ModePlayer modePlayer;

        std::string name;

        //physics movement
        float speed;
        float maxSpeed;
        float jumpSpeed;
        float dyingSpeed;
        bool isRunning;
        bool isDucking;

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
        SpriteState previousState;

        // gamehud
        int lives;
        CharacterType powerUpItem;
        int initialLives = 5;

        bool creativeMode;
        bool invulnerableMode;
    public:
        Character(CharacterName characterName, ModePlayer mode, Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed, int initialLives);
        virtual ~Character() override;

        void setWorld(World* world);

        void update() override;
        void draw() override;
        virtual void updateCollisionBoxes() override = 0;

        bool transition(float deltaTime);
        bool isTransitioning() const;
        void movement(float deltaTime);

        CollisionType checkCollision(Sprite* sprite) override;
        CollisionType checkCollisionEnemy(Sprite* sprite);
        void collisionTile(Tile* tile);
        void collisionBlock(Block* block);
        void collisionEnemy(Enemy* enemy);

        virtual void transitionToSmall() = 0;
        virtual void transitionToSuper() = 0;
        virtual void transitionToFlower() = 0;

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

        GameHud* getGameHud() const;
        Map* getMap() const;
        World* getWorld() const;

        void drawGameHud() const;
        void setLives(int lives);
        int getLives() const;
        void addLives(int lives);
        void removeLives(int lives);

        void setPowerUpItem(CharacterType type);
        CharacterType getPowerUpItem() const;
        void releasePowerUpItem();
        void setInitialLives(int initialLives);

        json saveToJson() const override;
        void loadFromJson(const json& j) override;

        void setCreativeMode(bool creative);
        bool isCreativeMode() const;
        void setInvulnerableMode(bool invulnerable);
        bool isInvulnerableMode() const;
        void setCharacterName(CharacterName name);
        CharacterName getCharacterName() const;
        GameMode getGameMode() const;

        void copyState(const Character& other);
};

#endif