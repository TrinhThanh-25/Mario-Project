#include "Enemy/JumpingPiranhaPlant.h"
#include "Common/ResourceManager.h"

JumpingPiranhaPlant::JumpingPiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::JUMPING_PIRANHA_PLANT, pos, dim, vel, color),
    minY(pos.y - dim.y*4),
    maxY(pos.y),
    velocityUp(-400.0f),
    velocityDown(80.0f),
    goingUp(true),
    waiting(false),
    waitTime(0.8f),
    waitAcum(0.0f)
{
    setState(SpriteState::ACTIVE);
    auxiliaryState = SpriteState::INVULNERABLE;
    velocity.y = velocityUp;
    isFacingLeft = true;
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

    if (!waiting) {
        if (goingUp) {
            position.y += velocityUp * delta;
            if (position.y <= minY) {
                position.y = minY;
                goingUp = false;
                waiting = true;
            }
        } else {
            position.y += velocityDown * delta;
            if (position.y >= maxY) {
                position.y = maxY;
                goingUp = true;
                waiting = true;
            }
        }
    } else {
        waitAcum += delta;
        if (waitAcum >= waitTime) {
            waitAcum = 0.0f;
            waiting = false;
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
    DrawTexture(tex, static_cast<int>(position.x), static_cast<int>(position.y), WHITE);

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
    // Không có collisionTile
}

void JumpingPiranhaPlant::collisionBlock(Block* block) {
    // Không có collisionBlock
}

// ======================= SAVE GAME =========================
json JumpingPiranhaPlant::saveToJson() const {
    json j = Enemy::saveToJson();
    j["minY"] = minY;
    j["maxY"] = maxY;
    j["velocityUp"] = velocityUp;
    j["velocityDown"] = velocityDown;
    j["goingUp"] = goingUp;
    j["waiting"] = waiting;
    j["waitTime"] = waitTime;
    j["waitAcum"] = waitAcum;
    return j;
}

void JumpingPiranhaPlant::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);
    minY = j["minY"].get<float>();
    maxY = j["maxY"].get<float>();
    velocityUp = j["velocityUp"].get<float>();
    velocityDown = j["velocityDown"].get<float>();
    goingUp = j["goingUp"].get<bool>();
    waiting = j["waiting"].get<bool>();
    waitTime = j["waitTime"].get<float>();
    waitAcum = j["waitAcum"].get<float>();
}

