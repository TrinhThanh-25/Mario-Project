#include "Enemy/PiranhaPlant.h"
#include "Common/ResourceManager.h"


PiranhaPlant::PiranhaPlant(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::PIRANHA_PLANT, pos, dim, vel, color)
{
    // Piranha luôn đứng yên tại chỗ (không cần gravity hay movement)
    setState(SpriteState::INACTIVE);          // ACTIVE để tham gia vòng lặp update
    piranhaState = PiranhaState::HIDING;

    hiddenY = pos.y;                        // Vị trí y khi ẩn hoàn toàn
    shownY = pos.y - dim.y;                 // Vị trí y khi trồi lên (tùy dim của bạn)

    riseSpeed = 30.0f;                      // Tốc độ trồi lên/hạ xuống (có thể chỉnh)
    stateDuration = 1.5f;                   // Thời gian delay giữa các pha (có thể chỉnh)
    stateTimer = 0.0f;

    position.y = hiddenY;                   // Bắt đầu ở trạng thái ẩn
    velocity = { 0, 0 };                    // Không di chuyển theo X/Y
    isFacingLeft = true;                    // Không quan trọng, vẫn cần nếu dùng chung Enemy
    type = EnemyType::PIRANHA_PLANT;
}


void PiranhaPlant::draw() {
    std::string textureKey;

    if (piranhaState == PiranhaState::HIDING || piranhaState == PiranhaState::LOWERING) {
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
    stateTimer += delta;

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
        return; // Bỏ qua update logic khác nếu đang dying
    }


    if (piranhaState == PiranhaState::HIDING){
        if (stateTimer >= stateDuration){
            piranhaState = PiranhaState::RISING;
            stateTimer = 0.0f;
        }
    }

    else if (piranhaState == PiranhaState::RISING){
        position.y -= riseSpeed * delta;
        if (position.y <= shownY){
            position.y = shownY;
            piranhaState = PiranhaState::SHOWN;
            stateTimer = 0.0f;
        }
    }

    else if (piranhaState == PiranhaState::SHOWN){
        if (stateTimer >= stateDuration){
            piranhaState = PiranhaState::LOWERING;
            stateTimer = 0.0f;
        }
    }

    else if (piranhaState == PiranhaState::LOWERING){
        position.y += riseSpeed * delta;
        if (position.y >= hiddenY){
            position.y = hiddenY;
            piranhaState = PiranhaState::HIDING;
            stateTimer = 0.0f;
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
    Enemy::collisionTile(tile);
}

void PiranhaPlant::collisionBlock(Block* block) {
    Enemy::collisionBlock(block);
}

// =========================== SAVE GAME ===========================
json PiranhaPlant::saveToJson() const {
    json j = Enemy::saveToJson();

    j["piranhaState"] = static_cast<int>(piranhaState);
    j["stateTimer"] = stateTimer;
    j["stateDuration"] = stateDuration;
    j["riseSpeed"] = riseSpeed;
    j["hiddenY"] = hiddenY;
    j["shownY"] = shownY;

    return j;
}

void PiranhaPlant::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    piranhaState = static_cast<PiranhaState>(j["piranhaState"].get<int>());
    stateTimer = j["stateTimer"].get<float>();
    stateDuration = j["stateDuration"].get<float>();
    riseSpeed = j["riseSpeed"].get<float>();
    hiddenY = j["hiddenY"].get<float>();
    shownY = j["shownY"].get<float>();
}
