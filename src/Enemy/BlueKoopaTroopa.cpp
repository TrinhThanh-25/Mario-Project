#include "Enemy/BlueKoopaTroopa.h"
#include "Common/ResourceManager.h"
#include <string>

#define WAKE_UP_TIME 5.0f

BlueKoopaTroopa::BlueKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {
    
    extraWakeUpTime = 2.0f;
    shellSpeed = 150.0f;
    shellTimer = 0.0f;
    shellMoving = false;

    setState(SpriteState::ACTIVE);
    isFacingLeft = vel.x < 0;
}

void BlueKoopaTroopa::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE) {
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // if (checkCollision(collidables) != CollisionType::NONE) {
        //     velocity.x = -velocity.x;
        //     isFacingLeft = velocity.x < 0;
        // }

        updateCollisionBoxes();
    }

    else if (state == SpriteState::SHELL) {
        velocity.y += 981.0f * delta;
        position.y += velocity.y * delta;

        if (checkCollision(collidables) == CollisionType::SOUTH) {
            velocity.y = 0;
        }

        shellTimer += delta;
        updateCollisionBoxes();

        if (shellTimer >= WAKE_UP_TIME + extraWakeUpTime) {
            setState(SpriteState::ACTIVE);
            velocity.x = isFacingLeft ? -40.0f : 40.0f;
            shellTimer = 0.0f;
        }
    }

    else if (state == SpriteState::SHELL_MOVING) {
        float dir = isFacingLeft ? -1.0f : 1.0f;
        position.x += shellSpeed * dir * delta;

        if (checkCollision(collidables) != CollisionType::NONE) {
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

void BlueKoopaTroopa::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "BlueKoopaTroopa0Left" : "BlueKoopaTroopa1Left")
                                  : (frame == 0 ? "BlueKoopaTroopa0Right" : "BlueKoopaTroopa1Right");
    }

    // else if (state == SpriteState::SHELL || state == SpriteState::SHELL_MOVING) {
    // textureKey = isFacingLeft ? "BlueKoopaShellLeft" : "BlueKoopaShellRight";
    // }

    // Bạn có thể thêm shell / dying nếu có sprite riêng
    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "BlueKoopaTroopa1Left" : "BlueKoopaTroopa1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}

void BlueKoopaTroopa::beingHit(HitType type) {
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
        case HitType::SHELL_KICK:
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
            break;

        default:
            break;
    }
}

void BlueKoopaTroopa::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        setState(SpriteState::SHELL_MOVING);
        shellMoving = true;
        isFacingLeft = faceLeft;
        shellTimer = 0.0f;
    }
}

bool BlueKoopaTroopa::isShellMoving() const {
    return shellMoving;
}

void BlueKoopaTroopa::activeWhenMarioApproach(Mario& mario) {
    // Green Koopa luôn ACTIVE từ đầu → không cần xử lý gì ở đây
}
