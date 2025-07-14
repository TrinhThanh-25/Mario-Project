#include "Enemy/BuzzyBeetle.h"
#include "Common/ResourceManager.h"
#include <string>

#define ACTIVATION_RANGE 300.0f
#define WAKE_UP_TIME 5.0f // tự bật dậy sau 5s

BuzzyBeetle::BuzzyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color), shellMoving(false), shellSpeed(200.0f),
      shellTimer(0.0f) {
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;
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

void BuzzyBeetle::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::ACTIVE) {
        // Nếu có leader, cập nhật hướng theo leader
        if (leader) {
            followTheLeader(leader);
        }

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
        shellTimer += delta;
        if (shellTimer >= WAKE_UP_TIME) {
            setState(SpriteState::ACTIVE);
            velocity.x = isFacingLeft ? -50.0f : 50.0f;
            shellTimer = 0.0f;
        }
    }

    else if (state == SpriteState::SHELL_MOVING) {
        velocity.y += 981.0f * delta;
        float dir = isFacingLeft ? -1.0f : 1.0f;
        position.x += shellSpeed * dir * delta;
        position.y += velocity.y * delta;

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

void BuzzyBeetle::activeWhenMarioApproach(Character& character) {
    Enemy::activeWhenMarioApproach(character);
}

void BuzzyBeetle::collisionSound() {
    // PlaySound(ResourceManager::getSound()["BuzzyHit"]);
}

void BuzzyBeetle::followTheLeader(Sprite* leader) {
    if (!leader || state != SpriteState::ACTIVE) return;

    Vector2 leaderPos = leader->getPosition();
    float delta = GetFrameTime();

    if (fabs(position.x - leaderPos.x) > 32.0f) {
        isFacingLeft = leaderPos.x < position.x;
        velocity.x = isFacingLeft ? -35.0f : 35.0f;

        position.x += velocity.x * delta;
        updateCollisionBoxes();
    }
}

void BuzzyBeetle::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    // Gọi base xử lý va chạm cơ bản
    Enemy::collisionTile(tile);

    if ((state == SpriteState::ACTIVE || state == SpriteState::SHELL_MOVING) &&
        (col == CollisionType::WEST || col == CollisionType::EAST)) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

void BuzzyBeetle::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    // Gọi base xử lý va chạm cơ bản
    Enemy::collisionBlock(block);

    if ((state == SpriteState::ACTIVE || state == SpriteState::SHELL_MOVING) &&
        (col == CollisionType::WEST || col == CollisionType::EAST)) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

