#ifndef MARIO_H
#define MARIO_H

#include "Mario/Fireball.h"
#include "Common/Sprite.h"
#include "Common/Direction.h"
#include "Common/SpriteState.h"
#include "Common/CollisionType.h"
#include "Common/CollisionBox.h"
#include "Mario/MarioType.h"
#include "raylib.h"
#include <vector>

class World;
class Map;

class Mario : public Sprite {
    private:
        World* world;
        Map* map;

        //physics movement
        float speed;
        float maxSpeed;
        float jumpSpeed;
        float dyingSpeed;
        bool isRunning;
        bool isDucking;
        bool isLookingUp;
        int lives;

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
        MarioType type;
        std::vector<Fireball> fireball;

        float activateWidth;
        int live;
        int coin;
        int yoshiCoin;

        // bool playerDownMusicStreamPlaying;
        // bool gameOverMusicStreamPlaying;
        // SpriteState lastStateBeforeTransition;
    public:
        Mario(Vector2 pos, Vector2 dim, Vector2 vel, Color color, float speedX, float maxSpeedX, float jumpSpeed);
        ~Mario() override;

        void update() override;
        void draw() override;
        void updateCollisionBoxes() override;

        bool transition(float deltaTime);
        void movement(float deltaTime);

        //CollisionType checkCollision(Sprite* sprite) const override;
        void collisionTile(Sprite* sprite) const;
        void collisionBlock(Sprite* sprite) const;
        void collisionEnemy(Sprite* sprite) const;

        void setLives(int lives);
        int getLives() const;

        void transitionToSmall();
        void transitionToSuper();
        void transitionToFlower();

        void setInvulnerable(bool invulnerable);
        bool isInvulnerable() const;
        void setInvincible(bool invincible);
        bool isInvincible() const;

        void setType(MarioType type);
        MarioType getType() const;
};

#endif