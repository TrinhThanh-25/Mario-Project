#include "Enemy/MummyBeetle.h"
#include "Common/ResourceManager.h"
#include <string>

#define ACTIVATION_RANGE 300.0f
#define WAKE_UP_TIME 5.0f // tự bật dậy sau 5s

MummyBeetle::MummyBeetle(Vector2 pos, Vector2 dim, Vector2 vel, Color color) 
    : Enemy(EnemyType::MUMMY_BEETLE, pos, dim, vel, color) {
    extraWakeUpTime = 2.0f;
    shellSpeed = 150.0f;
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;   
    shellMoving = false;
    type = EnemyType::MUMMY_BEETLE;
    point = 100;
}

void MummyBeetle::update(const std::vector<Character*>& characterList) {
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
        shellTimer += delta;
        velocity.y += World::gravity * delta;
        position.y += velocity.y * delta;
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

void MummyBeetle::draw(){
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "MummyBeetle0Left" : "MummyBeetle1Left")
                                  : (frame == 0 ? "MummyBeetle0Right" : "MummyBeetle1Right");
    } 

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    // else if (state == SpriteState::SHELL || state == SpriteState::SHELL_MOVING || state == SpriteState::DYING) {
    //     textureKey = isFacingLeft ? "BuzzyBeetleShellLeft" : "BuzzyBeetleShellRight";
    // }

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "MummyBeetle1Left" : "MummyBeetle1Right";
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


void MummyBeetle::beingHit(HitType type) {
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

void MummyBeetle::kickShell(bool faceLeft){
    if (state == SpriteState::SHELL) {
        setState(SpriteState::SHELL_MOVING);
        shellMoving = true;
        isFacingLeft = faceLeft;
        shellTimer = 0.0f;
    }
}

bool MummyBeetle::isShellMoving() const {
    return shellMoving;
}

void MummyBeetle::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void MummyBeetle::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;

        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
        // SHELL_MOVING không cần chỉnh velocity, chỉ cần isFacingLeft
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}


void MummyBeetle::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);
    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;

        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}


// ============================= SAVE GAME ===============================
json MummyBeetle::saveToJson() const {
    json j = Enemy::saveToJson();

    j["shellMoving"] = shellMoving;
    j["shellTimer"] = shellTimer;
    j["shellSpeed"] = shellSpeed;
    j["extraWakeUpTime"] = extraWakeUpTime;

    return j;
}

void MummyBeetle::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    shellMoving = j["shellMoving"].get<bool>();
    shellTimer = j["shellTimer"].get<float>();
    shellSpeed = j["shellSpeed"].get<float>();
    extraWakeUpTime = j["extraWakeUpTime"].get<float>();
}
