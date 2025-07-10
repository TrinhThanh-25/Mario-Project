#include "Enemy/Rex.h"
#include "Common/ResourceManager.h"
#include <cmath>
#include <string>

#define ACTIVATION_RANGE 300.0f

Rex::Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color), isShrunken(false), shrinkDuration(0.0f) {
    isFacingLeft = true;
}

void Rex::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();
    if (state == SpriteState::INACTIVE){
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE){
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        if (checkCollision(collidables) != CollisionType::NONE) {
            velocity.x = -velocity.x;
            isFacingLeft = velocity.x < 0;
        }

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
void Rex::draw() {
    std::string textureKey;
    int frame = (int) (GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE){
        if (isShrunken){
            if (isFacingLeft){
                textureKey = (frame == 0) ? "Rex10Left" : "Rex11Left";
            }
            else {
                textureKey = (frame == 0) ? "Rex10Right" : "Rex11Right";
            }
        }

        else {
            if (isFacingLeft){
                textureKey = (frame == 0) ? "Rex20Left" : "Rex21Left";
            }
            else {
                textureKey = (frame == 0) ? "Rex20Right" : "Rex21Right";
            }
        }
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);     
    }

    if (state == SpriteState::DYING) {
        std::string dyingKey;

        // Dùng hình Rex lùn (1) khi chết – giống behavior trong Super Mario World
        if (isFacingLeft) {
            dyingKey = "Rex10Left";
        } 
        else {
            dyingKey = "Rex10Right";
        }

        // Vẽ hình chết
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        // Vẽ điểm bay lên
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }

}

void Rex::beingHit(HitType type) {
    if (type == HitType::STOMP){
        if (!isShrunken){
            isShrunken = true;
            velocity.x = 0;
        }
        else {
            setState(SpriteState::DYING);
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            diePosition = position;
        }
    }
    else if (type == HitType::FIREBALL || type == HitType::SHELL_KICK){
        setState(SpriteState::DYING);
        currentDyingFrame = 0;
        dyingFrameAcum = 0.0f;
        diePosition = position;
    }
}

void Rex::activeWhenMarioApproach(Mario& mario) {
    if (state != SpriteState::INACTIVE) return;

    Vector2 marioPos = mario.getPosition();
    float dx = fabs(marioPos.x - position.x);

    if (dx <= ACTIVATION_RANGE) {
        setState(SpriteState::ACTIVE);
        velocity.x = isFacingLeft ? -30.0f : 30.0f;
    }
}
