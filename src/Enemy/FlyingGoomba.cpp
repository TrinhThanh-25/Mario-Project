
#include "Enemy/FlyingGoomba.h"
#include "Common/ResourceManager.h"


FlyingGoomba::FlyingGoomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::FLYING_GOOMBA, pos, dim, vel, color){
        
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;   
    type = EnemyType::FLYING_GOOMBA;
    point = 200;
    baseY = pos.y;
}

FlyingGoomba::~FlyingGoomba() {
    // Destructor logic if needed
}

void FlyingGoomba::draw() {
    std::string textureKey;

    if (state == SpriteState::ACTIVE) {
        int frame = (int)(flyingCycleTime * 6) % 2; // dùng flyingCycleTime cho đồng bộ vỗ cánh

        if (movetype == MoveType::FLYING) {
            if (isFacingLeft) {
                textureKey = (frame == 0) ? "FlyingGoomba0Left" : "FlyingGoomba1Left";
            } else {
                textureKey = (frame == 0) ? "FlyingGoomba0Right" : "FlyingGoomba1Right";
            }
        } else if (movetype == MoveType::WALKING) {
            frame = (int)(GetTime() * 6) % 2; // walking vẫn dùng thời gian thực
            if (isFacingLeft) {
                textureKey = (frame == 0) ? "Goomba0Left" : "Goomba1Left";
            } else {
                textureKey = (frame == 0) ? "Goomba0Right" : "Goomba1Right";
            }
        }

        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }

    else if (state == SpriteState::DYING) {
        std::string dyingKey;
        if (movetype == MoveType::WALKING) {
            dyingKey = isFacingLeft ? "Goomba1Left" : "Goomba1Right";
        } else {
            dyingKey = isFacingLeft ? "FlyingGoomba1Left" : "FlyingGoomba1Right";
        }

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
                (float)guiTex.height},
            Vector2{guiTex.width / 2.0f, guiTex.height / 2.0f},
            angle,
            WHITE);
    }
}



void FlyingGoomba::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::ACTIVE) {
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        position.x += velocity.x * delta;

        if (movetype == MoveType::FLYING) {
            // Cập nhật bay hình sin
            flyingCycleTime += delta;
            if (flyingCycleTime > flyingCycleDuration)
                flyingCycleTime -= flyingCycleDuration;

            float phase = (flyingCycleTime / flyingCycleDuration) * 2 * PI;
            position.y = baseY + sin(phase) * flyingAmplitude;
        }
        else if (movetype == MoveType::WALKING) {
            position.y += velocity.y * delta;
            velocity.y += World::gravity * delta;
        }

        updateCollisionBoxes();
    }

    else if (state == SpriteState::DYING) {
        dyingFrameAcum += delta;
        if (dyingFrameAcum >= dyingFrameTime) {
            currentDyingFrame++;
            dyingFrameAcum = 0.0f;

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


void FlyingGoomba::beingHit(HitType type){
    if (type == HitType::STOMP){
        if (movetype == MoveType::FLYING){
            setMoveType(MoveType::WALKING);
            velocity.y = 0;
            baseY = position.y;

            float previousHeight = size.y;
            setSize({32, 32});
            position.y += previousHeight - 32.0f;
            updateCollisionBoxes();
        }

        else if (movetype == MoveType::WALKING){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
            velocity = {0, 0};
        }
    }
    else {
        if (state == SpriteState::ACTIVE || state == SpriteState::INACTIVE) {
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
            velocity = {0, 0};
        }
    }

}
    
void FlyingGoomba::collisionSound(){

}

void FlyingGoomba::activeWhenMarioApproach(Character& character){
    float distance = std::abs(character.getPosition().x - position.x);
    if (distance < 3200.0f) {
        setState(SpriteState::ACTIVE);
        setMoveType(MoveType::FLYING);
    }
}

void FlyingGoomba::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    Enemy::collisionTile(tile);

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

void FlyingGoomba::collisionBlock(Block* block) {
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

// ============================= SAVE GAME ============================
json FlyingGoomba::saveToJson() const {
    json j = Enemy::saveToJson();  // Gọi hàm cha

    j["jumpTimer"] = jumpTimer;
    j["jumpInterval"] = jumpInterval;
    j["jumpSpeed"] = jumpSpeed;

    return j;
}

void FlyingGoomba::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);  // Gọi hàm cha

    jumpTimer = j["jumpTimer"].get<float>();
    jumpInterval = j["jumpInterval"].get<float>();
    jumpSpeed = j["jumpSpeed"].get<float>();
}
