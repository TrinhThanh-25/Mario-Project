#include "Enemy/JumpingPiranhaPlant.h"
#include "Common/ResourceManager.h"

JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::JUMPING_PIRANHA_PLANT, pos, dim, vel, color) 
{
    setState(SpriteState::INACTIVE);              // Luôn hoạt động
    jumpState = JumpingPiranhaState::IDLE;

    groundY = pos.y;                            // Mặt ống – vị trí đứng ban đầu
    waitDuration = 2.0f;                        // Chờ 2s giữa các cú nhảy
    waitTimer = 0.0f;

    jumpSpeed = 200.0f;                         // Tốc độ nhảy lên                    

    velocity = {0, 0};                          // Bắt đầu đứng yên
    isFacingLeft = true;                        // Không quan trọng nhưng giữ cho đồng bộ
    type = EnemyType::JUMPING_PIRANHA_PLANT;
    maxJumpHeight = 64.0f;
    point = 200;
}

    
void JumpingPiranhaPlant::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (jumpState == JumpingPiranhaState::IDLE) {
        waitTimer += delta;
        if (waitTimer >= waitDuration) {
            velocity.y = -jumpSpeed;
            jumpState = JumpingPiranhaState::JUMPING;
            waitTimer = 0.0f;
        }
    }

    else if (jumpState == JumpingPiranhaState::JUMPING) {
        velocity.y += gravity * delta;
        position.y += velocity.y * delta;

         float jumpPeakY = groundY - maxJumpHeight;
        if (position.y <= jumpPeakY) {
            position.y = jumpPeakY;
            velocity.y = 0;
            jumpState = JumpingPiranhaState::FALLING;
        }

        if (velocity.y >= 0) {
            jumpState = JumpingPiranhaState::FALLING;
        }
    }

    else if (jumpState == JumpingPiranhaState::FALLING) {
        velocity.y += World::gravity * delta;
        position.y += velocity.y * delta;

        if (position.y >= groundY) {
            position.y = groundY;
            velocity.y = 0;
            jumpState = JumpingPiranhaState::IDLE;
            waitTimer = 0.0f;
        }
    }

    updateCollisionBoxes();
    if (state == SpriteState::DYING) {
        float delta = GetFrameTime();

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
    int frame = (int)(GetTime() * 6) % 2;

    if (jumpState == JumpingPiranhaState::IDLE || jumpState == JumpingPiranhaState::FALLING) {
        textureKey = frame == 0 ? "JumpingPiranhaPlant2" : "JumpingPiranhaPlant3";
    } else if (jumpState == JumpingPiranhaState::JUMPING) {
        textureKey = frame == 0 ? "JumpingPiranhaPlant0" : "JumpingPiranhaPlant1";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

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

    j["jumpState"] = static_cast<int>(jumpState);
    j["jumpSpeed"] = jumpSpeed;
    j["gravity"] = gravity;
    j["waitDuration"] = waitDuration;
    j["waitTimer"] = waitTimer;
    j["groundY"] = groundY;
    j["maxJumpHeight"] = maxJumpHeight;

    return j;
}

void JumpingPiranhaPlant::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    jumpState = static_cast<JumpingPiranhaState>(j["jumpState"].get<int>());
    jumpSpeed = j["jumpSpeed"].get<float>();
    gravity = j["gravity"].get<float>();
    waitDuration = j["waitDuration"].get<float>();
    waitTimer = j["waitTimer"].get<float>();
    groundY = j["groundY"].get<float>();
    maxJumpHeight = j["maxJumpHeight"].get<float>();
}
