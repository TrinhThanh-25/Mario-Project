#include "Enemy/PiranhaPlant.h"
#include "Common/ResourceManager.h"


PiranhaPlant::PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::PIRANHA_PLANT, pos, dim, vel, color),
    minY(pos.y - dim.y),
    maxY(pos.y),
    velocityUp(80.0f),
    waitTime(2.0f), 
    waitAcum(0.0f),
    waiting(false)
{
    // Piranha luôn đứng yên tại chỗ (không cần gravity hay movement)
    setState(SpriteState::INACTIVE);
    auxiliaryState = SpriteState::INVULNERABLE;
    velocity = { 0, 0 };
    isFacingLeft = true;
    type = EnemyType::PIRANHA_PLANT;
    point = 200;
}


void PiranhaPlant::draw() {
    std::string textureKey;
    int frame = static_cast<int>(GetTime() * 6) % 2;
    if (frame == 0) {
        textureKey = "PiranhaPlant1";
    } else {
        textureKey = "PiranhaPlant0";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
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



void PiranhaPlant::update(const std::vector<Character*>& characterList){

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }
    
    float delta = GetFrameTime();

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

    frameAcum += delta;
    if (frameAcum >= frameTime) {
        frameAcum = 0.0f;
        curFrame++;
        curFrame %= maxFrame;
    }
    if(!waiting) {
        position.y += velocityUp * delta;
        if (position.y <= minY) {
            position.y = minY;
            waiting = true;
            velocityUp *= -1;
        } else if (position.y >= maxY) {
            position.y = maxY;
            waiting = true;
            velocityUp *= -1;
        }
    }
    else {
        waitAcum += delta;
        if (waitAcum >= waitTime) {
            waitAcum = 0.0f;
            waiting = false;
        }
    }
    updateCollisionBoxes();
}

void PiranhaPlant::beingHit(HitType type){
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

void PiranhaPlant::collisionSound(){

}
    
void PiranhaPlant::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void PiranhaPlant::collisionTile(Tile* tile) {
    
}

void PiranhaPlant::collisionBlock(Block* block) {
    
}

// =========================== SAVE GAME ===========================
json PiranhaPlant::saveToJson() const {
    json j = Enemy::saveToJson();
    j["minY"] = minY;
    j["maxY"] = maxY;
    j["velocityUp"] = velocityUp;
    j["waitTime"] = waitTime;
    j["waitAcum"] = waitAcum;
    j["waiting"] = waiting;
    return j;
}

void PiranhaPlant::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);
    minY = j["minY"].get<float>();
    maxY = j["maxY"].get<float>();
    velocityUp = j["velocityUp"].get<float>();
    waitTime = j["waitTime"].get<float>();
    waitAcum = j["waitAcum"].get<float>();
    waiting = j["waiting"].get<bool>();
}
