#include "Enemy/BanzaiBill.h"
#include "Common/ResourceManager.h"
#include <cmath>
#include <string>


BanzaiBill::BanzaiBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {
    
    setState(SpriteState::INACTIVE);   // Chờ Mario đến gần mới active
    isFacingLeft = vel.x < 0;     // Dựa theo hướng bay ban đầu

    maxLifeTime = 8.0f;    // Sau 8 giây sẽ tự biến mất (có thể tùy chỉnh)
    lifeTimer = 0.0f;
}

void BanzaiBill::update(const std::vector<Character*>& characterList) {
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

        updateCollisionBoxes();

        lifeTimer += delta;
        if (lifeTimer >= maxLifeTime) {
            setState(SpriteState::TO_BE_REMOVED);
        }

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


void BanzaiBill::draw() {
    std::string textureKey;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ?  "BanzaiBill0Left" :  "BanzaiBill0Right";
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);                  
    } 

    if (state == SpriteState::DYING) {

        std::string dyingKey = isFacingLeft ? "BanzaiBill1Left" :  "BanzaiBill0Right"; 
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}

void BanzaiBill::beingHit(HitType type){
    if (type == HitType::STOMP){
        return;
    }

    else if (type == HitType::FIREBALL){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
        }
    }
}


void BanzaiBill::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void BanzaiBill::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }
}

void BanzaiBill::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }
}
