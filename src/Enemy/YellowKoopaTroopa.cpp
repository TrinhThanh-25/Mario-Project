#include "Enemy/YellowKoopaTroopa.h"
#include "Common/ResourceManager.h"
#include <string>

#define WAKE_UP_TIME 5.0f

YellowKoopaTroopa::YellowKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::YELLOW_KOOPA_TROOPA, pos, dim, vel, color) {
    
    extraWakeUpTime = 2.0f;
    shellSpeed = 150.0f;
    shellTimer = 0.0f;
    shellMoving = false;

    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;
    type = EnemyType::YELLOW_KOOPA_TROOPA;

    point = 100;
}

void YellowKoopaTroopa::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::ACTIVE) {
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        velocity.y += World::gravity * delta;


        updateCollisionBoxes();
    }

    else if (state == SpriteState::SHELL) {
        velocity.y += World::gravity * delta;
        position.y += velocity.y * delta;

        shellTimer += delta;
        updateCollisionBoxes();

        if (shellTimer >= WAKE_UP_TIME + extraWakeUpTime) {
            setState(SpriteState::ACTIVE);
            velocity.x = isFacingLeft ? -50.0f : 50.0f;
            shellTimer = 0.0f;
        }
    }

    else if (state == SpriteState::SHELL_MOVING) {
        float dir = isFacingLeft ? -1.0f : 1.0f;
        position.x += shellSpeed * dir * delta;

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

void YellowKoopaTroopa::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "YellowKoopa0Left" : "YellowKoopa1Left")
                                  : (frame == 0 ? "YellowKoopa0Right" : "YellowKoopa1Right");
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "YellowKoopa1Left" : "YellowKoopa1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        float angle = sin(GetTime() * 10.0f) * 10.0f;

        Texture2D& guiTex = ResourceManager::getTexture()["Gui100"];
        DrawTexturePro(
            guiTex,
            Rectangle{0, 0, (float)guiTex.width, (float)guiTex.height},
            Rectangle{
                diePosition.x,
                diePosition.y - offsetY,
                (float)guiTex.width,
                (float)guiTex.height
            },
            Vector2{guiTex.width / 2.0f, guiTex.height / 2.0f},
            angle,
            WHITE
        );
    }
}


void YellowKoopaTroopa::beingHit(HitType type) {
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

void YellowKoopaTroopa::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        setState(SpriteState::SHELL_MOVING);
        shellMoving = true;
        isFacingLeft = faceLeft;
        shellTimer = 0.0f;
    }
}

bool YellowKoopaTroopa::isShellMoving() const {
    return shellMoving;
}

void YellowKoopaTroopa::activeWhenMarioApproach(Character& character) {
    Enemy::activeWhenMarioApproach(character);
}

void YellowKoopaTroopa::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

void YellowKoopaTroopa::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

// ============================= SAVE GAME ==================================
json YellowKoopaTroopa::saveToJson() const {
    json j = Enemy::saveToJson();

    j["shellMoving"] = shellMoving;
    j["shellTimer"] = shellTimer;
    j["shellSpeed"] = shellSpeed;
    j["extraWakeUpTime"] = extraWakeUpTime;

    return j;
}

void YellowKoopaTroopa::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    shellMoving = j["shellMoving"].get<bool>();
    shellTimer = j["shellTimer"].get<float>();
    shellSpeed = j["shellSpeed"].get<float>();
    extraWakeUpTime = j["extraWakeUpTime"].get<float>();
}
