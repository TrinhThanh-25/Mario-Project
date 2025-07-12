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

void BanzaiBill::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
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
        textureKey = isFacingLeft ?  "BobOmb0Left" :  "BobOmb0Right";
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);                  
    } 

    if (state == SpriteState::DYING) {

        std::string dyingKey = isFacingLeft ? "BobOmb1Left" :  "BanzaiBill0Right"; 
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


void BanzaiBill::activeWhenMarioApproach(Mario& mario) {
    Vector2 marioPos = mario.getPosition();
    float dx = fabs(marioPos.x - position.x);

    if (dx <= 300.0f) { // hoặc khoảng cách bạn mong muốn
        setState(SpriteState::ACTIVE);
    }
}
