#include "Enemy/BuzzyBeetle.h"
#include "Common/ResourceManager.h"
#include <string>

#define ACTIVATION_RANGE 300.0f
#define WAKE_UP_TIME 5.0f // tự bật dậy sau 5s

BuzzyBeetle::BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color), shellMoving(false), shellSpeed(200.0f),
      shellTimer(0.0f), isFacingLeft(true) {
    setState(SpriteState::INACTIVE);
}

void BuzzyBeetle::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "BuzzyBeetle0Left" : "BuzzyBeetle1Left")
                                  : (frame == 0 ? "BuzzyBeetle0Right" : "BuzzyBeetle1Right");
    } 
    // else if (state == SpriteState::SHELL || state == SpriteState::SHELL_MOVING || state == SpriteState::DYING) {
    //     textureKey = isFacingLeft ? "BuzzyBeetleShellLeft" : "BuzzyBeetleShellRight";
    // }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {

        std::string dyingKey = isFacingLeft ? "BuzzyBeetle0Left" : "BuzzyBeetle0Right"; 
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}

void BuzzyBeetle::update(Mario& mario, const std::vector<Sprite*>& mapCollidables) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE) {
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        if (checkCollision(mapCollidables) != CollisionType::NONE) {
            velocity.x = -velocity.x;
            isFacingLeft = velocity.x < 0;
        }

        updateCollisionBoxes();
    }

    else if (state == SpriteState::SHELL) {
        shellTimer += delta;
        if (shellTimer >= WAKE_UP_TIME) {
            setState(SpriteState::ACTIVE);
            velocity.x = isFacingLeft ? -50.0f : 50.0f;
            shellTimer = 0.0f;
        }
    }

    else if (state == SpriteState::SHELL_MOVING) {
        float dir = isFacingLeft ? -1.0f : 1.0f;
        position.x += shellSpeed * dir * delta;

        if (checkCollision(mapCollidables) != CollisionType::NONE) {
            isFacingLeft = !isFacingLeft;
        }

        updateCollisionBoxes();
    }

    else if (state == SpriteState::DYING) {
        dyingFrameAcum += delta;
        if (dyingFrameAcum >= dyingFrameTime) {
            dyingFrameAcum = 0.0f;
            currentDyingFrame++;
            if (currentDyingFrame >= maxDyingFrame) {
                setState(SpriteState::TO_BE_REMOVED);
            }
        }
        pointFrameAcum += delta;
        if (pointFrameAcum >= pointFrameTime) {
            pointFrameAcum = pointFrameTime;
        }
    }
}


void BuzzyBeetle::beingHit(HitType type) {
    switch (type) {
        case HitType::STOMP:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::SHELL);
                velocity.x = 0;
                shellMoving = false;
                shellTimer = 0.0f;
            }
            else if (state == SpriteState::SHELL) {
                setState(SpriteState::SHELL_MOVING);
                shellMoving = true;
                shellTimer = 0.0f;
            }
            else if (state == SpriteState::SHELL_MOVING) {
                setState(SpriteState::SHELL);
                shellMoving = false;
                shellTimer = 0.0f;
            }
            break;

        case HitType::FIREBALL:
            // Không bị ảnh hưởng
            break;

        case HitType::SHELL_KICK:
            if (state != SpriteState::DYING && state != SpriteState::TO_BE_REMOVED) {
                setState(SpriteState::DYING);
                diePosition = position;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                shellMoving = false;
            }
            break;

        default:
            break;
    }
}

void BuzzyBeetle::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        setState(SpriteState::SHELL_MOVING);
        shellMoving = true;
        isFacingLeft = faceLeft;
        shellTimer = 0.0f;
    }
}

bool BuzzyBeetle::isShellMoving() const {
    return shellMoving;
}

void BuzzyBeetle::activeWhenMarioApproach(Mario& mario) {
    if (state != SpriteState::INACTIVE) return;

    Vector2 marioPos = mario.getPosition();
    float dx = fabs(marioPos.x - position.x);

    if (dx <= ACTIVATION_RANGE) {
        setState(SpriteState::ACTIVE);
        velocity.x = isFacingLeft ? -50.0f : 50.0f;
    }
}

void BuzzyBeetle::collisionSound() {
    // PlaySound(ResourceManager::getSound()["BuzzyHit"]);
}
