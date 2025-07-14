#include "Enemy/Swooper.h"
#include "Common/ResourceManager.h"


Swooper::Swooper(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {
    
    setState(SpriteState::INACTIVE);     // Treo trên trần, chờ Mario đến gần
    isFacingLeft = vel.x < 0;                 

    isDropping = false;
    isFlyingHorizontally = false;

    dropSpeed = 120.0f;                  // Tốc độ rơi
    flySpeed = 60.0f;                    // Tốc độ bay ngang 
    activationRangeY = 100.0f;           // Khoảng cách dọc để kích hoạt
    startPosition = position;

}

void Swooper::update(const std::vector<Character*>& characterList){
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
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

    if (state == SpriteState::ACTIVE){
        // CollisionType col = checkCollision(collidables); // Tạm bỏ

        if (isDropping){
            position.y += velocity.y * delta;

            // Tạm thời giả định sau khi rơi một đoạn thì bắt đầu bay ngang
            if (position.y - startPosition.y > 50.0f) {
                isDropping = false;
                velocity.y = 0;
                isFlyingHorizontally = true;
                velocity.x = isFacingLeft ? -flySpeed : flySpeed;
            }
        }

        if (isFlyingHorizontally){
            position.x += velocity.x * delta;

            // Tạm thời đổi hướng nếu chạm rìa màn hình
            if (position.x < 0 || position.x > GetScreenWidth()) {
                velocity.x = -velocity.x;
                isFacingLeft = !isFacingLeft;
            }
        }

        updateCollisionBoxes();
    }
}


void Swooper::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::INACTIVE) {
        textureKey = isFacingLeft ? "Swooper0Left" : "Swooper0Right";
    }
    else if (state == SpriteState::ACTIVE) {
        if (frame == 0)
            textureKey = isFacingLeft ? "Swooper1Left" : "Swooper1Right";
        else
            textureKey = isFacingLeft ? "Swooper2Left" : "Swooper2Right";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "Swooper1Left" : "Swooper1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}
    
void Swooper::beingHit(HitType type) {
    if (state != SpriteState::ACTIVE) return;

    switch (type) {
        case HitType::STOMP:
        case HitType::FIREBALL:
        case HitType::SHELL_KICK:
            setState(SpriteState::DYING);
            dyingFrameAcum = 0.0f;
            diePosition = position;
            break;

        default:
            break;
    }
}

void Swooper::activeWhenMarioApproach(Character& character){
    if (state != SpriteState::INACTIVE) return;

    Vector2 marioPos = character.getPosition();  // <-- sửa dòng này
    float dx = abs(marioPos.x - position.x);
    float dy = abs(position.y - marioPos.y);

    if (dx <= 3200.0f && dy <= activationRangeY){
        setState(SpriteState::ACTIVE);
        isDropping = true;
        velocity.y = dropSpeed;
        startPosition = position;  // nhớ lưu vị trí bắt đầu rơi
    }
}


void Swooper::collisionBlock(Block* block){

}

void Swooper::collisionTile(Tile* tile){

}
