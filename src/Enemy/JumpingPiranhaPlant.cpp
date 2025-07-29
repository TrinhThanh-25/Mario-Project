#include "Enemy/JumpingPiranhaPlant.h"
#include "Common/ResourceManager.h"

JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::JUMPING_PIRANHA_PLANT, pos, dim, vel, color) 
{
    setState(SpriteState::ACTIVE);             

    groundY = pos.y;                         // Mặt ống – vị trí đứng ban                     
    minY = groundY - 64;
    jumpSpeed = 200.0f;                         // Tốc độ nhảy lên                    

    velocity = {0, 0};                          // Bắt đầu đứng yên
    isFacingLeft = true;                        // Không quan trọng nhưng giữ cho đồng bộ
    type = EnemyType::JUMPING_PIRANHA_PLANT;
    point = 200;
}

void JumpingPiranhaPlant::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;
        }
        if (state == SpriteState::INACTIVE) return;
    }

    // Handle DYING logic
    if (state == SpriteState::DYING) {
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
        return;
    }

    // === Jumping logic ===
    if (!waiting) {
        position.y += velocity.y * delta;

        if (goingUp) {
            velocity.y *= 0.96f;
            if (position.y <= minY) {
                position.y = minY;
                velocity.y = 0;
                goingUp = false;
                waiting = true;
                waitTimer = 0;
            }
        } else {
            velocity.y += World::gravity * delta;
            if (position.y >= groundY) {
                position.y = groundY;
                velocity.y = 0;
                goingUp = true;
                waiting = true;
                waitTimer = 0;
            }
        }
    } else {
        waitTimer += delta;
        if (waitTimer >= waitDuration) {
            waiting = false;
            if (goingUp) {
                velocity.y = -jumpSpeed;
            }
            else {
                velocity.y = 0;
            }
        }
    }
    updateCollisionBoxes();
}



void JumpingPiranhaPlant::beingHit(HitType type){
    if (type == HitType::STOMP) return;
    
    else if (type == HitType::FIREBALL || type == HitType::SHELL_KICK){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
        }
    }
}

void JumpingPiranhaPlant::draw() {
    std::string textureKey;

    // 4 frame quay vòng: mở/đóng + chân
    int frame = static_cast<int>(GetTime() * 6) % 4;
    const char* keys[] = {
        "JumpingPiranhaPlant0",
        "JumpingPiranhaPlant1",
        "JumpingPiranhaPlant2",
        "JumpingPiranhaPlant3"
    };
    textureKey = keys[frame];

    Texture2D& tex = ResourceManager::getTexture()[textureKey];

    // Vẽ tại gốc chân (position.y là mặt ống)
    DrawTexture(tex, static_cast<int>(position.x), static_cast<int>(position.y - tex.height), WHITE);

    // Vẽ điểm khi DYING
    if (state == SpriteState::DYING) {
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        float angle = sin(GetTime() * 10.0f) * 10.0f;

        Texture2D& guiTex = ResourceManager::getTexture()["Gui100"];
        DrawTexturePro(
            guiTex,
            Rectangle{ 0, 0, (float)guiTex.width, (float)guiTex.height },
            Rectangle{
                diePosition.x,
                diePosition.y - offsetY,
                (float)guiTex.width,
                (float)guiTex.height
            },
            Vector2{ guiTex.width / 2.0f, guiTex.height / 2.0f },
            angle,
            WHITE
        );
    }
}


void JumpingPiranhaPlant::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void JumpingPiranhaPlant::collisionTile(Tile* tile) {
    Enemy::collisionTile(tile);
}

void JumpingPiranhaPlant::collisionBlock(Block* block) {
    Enemy::collisionBlock(block);
}

// ======================= SAVE GAME =========================
json JumpingPiranhaPlant::saveToJson() const {
    json j = Enemy::saveToJson();

    // j["jumpState"] = static_cast<int>(jumpState);
    j["jumpSpeed"] = jumpSpeed;
    j["waitDuration"] = waitDuration;
    j["waitTimer"] = waitTimer;
    j["groundY"] = groundY;
    // j["maxJumpHeight"] = maxJumpHeight;

    return j;
}

void JumpingPiranhaPlant::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    // jumpState = static_cast<JumpingPiranhaState>(j["jumpState"].get<int>());
    jumpSpeed = j["jumpSpeed"].get<float>();
    waitDuration = j["waitDuration"].get<float>();
    waitTimer = j["waitTimer"].get<float>();
    groundY = j["groundY"].get<float>();
    // maxJumpHeight = j["maxJumpHeight"].get<float>();
}

