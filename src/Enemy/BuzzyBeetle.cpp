#include "Enemy/BuzzyBeetle.h"
#include <string>
#include "Common/ResourceManager.h"

BuzzyBeetle::BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {

    shellMoving = false;
    shellSpeed = 200.0f;
    isFacingLeft = true;
}



void BuzzyBeetle::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        if (isFacingLeft) {
            textureKey = (frame == 0) ? "BuzzyBeetle0Left" : "BuzzyBeetle1Left";
        } else {
            textureKey = (frame == 0) ? "BuzzyBeetle0Right" : "BuzzyBeetle1Right";
        }
    }
    else if (state == SpriteState::SHELL || state == SpriteState::DYING) {
        textureKey = isFacingLeft ? "BuzzyBeetle1Left" : "BuzzyBeetle1Right";
    }
    else {
        textureKey = isFacingLeft ? "BuzzyBeetle0Left" : "BuzzyBeetle0Right";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}




void BuzzyBeetle::update(Mario& mario) {
    float delta = GetFrameTime();

    // Nếu chưa active → kiểm tra Mario
    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE) {
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        updateCollisionBoxes();

        // Cập nhật hướng nhìn
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }
    }
    else if (state == SpriteState::SHELL) {
        if (shellMoving) {
            float dir = isFacingLeft ? -1.0f : 1.0f;
            position.x += shellSpeed * dir * delta;
            updateCollisionBoxes();
        }
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
    }

    // Point bay lên nếu đang dying
    if (state == SpriteState::DYING) {
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
            }
            else if (state == SpriteState::SHELL) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                shellMoving = false;
            }
            break;

        case HitType::FIREBALL:
            // BuzzyBettle miễn nhiễm Fireball
            break;

        case HitType::SHELL_KICK:
            // Trúng mai trượt → chết luôn
            if (state != SpriteState::DYING && state != SpriteState::TO_BE_REMOVED) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                shellMoving = false;
            }
            break;

        default:
            break;
    }
    // PlaySound(ResourceManager::getSound()["EnemyDefeat"]);
}


void BuzzyBeetle::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        shellMoving = true;
        isFacingLeft = faceLeft;
    }
}
 
bool BuzzyBeetle::isShellMoving() const {
    return shellMoving;
}

#define ACTIVATION_RANGE 300.0f

void BuzzyBeetle::activeWhenMarioApproach(Mario& mario) {
    if (state != SpriteState::INACTIVE) return;  // Đã active hoặc đã chết v.v.

    Vector2 marioPos = mario.getPosition();
    float dx = fabs(marioPos.x - position.x);

    if (dx <= ACTIVATION_RANGE) {
        setState(SpriteState::ACTIVE);
    }
}

void BuzzyBeetle::collisionSound(){
    // PlaySound(ResourceManager::getSound()["BuzzyHit"]);
}


