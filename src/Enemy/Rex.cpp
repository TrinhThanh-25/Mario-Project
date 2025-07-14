#include "Enemy/Rex.h"
#include "Common/ResourceManager.h"
#include <cmath>
#include <string>

#define ACTIVATION_RANGE 300.0f

Rex::Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color), isShrunken(false), shrinkDuration(0.0f) {
    isFacingLeft = vel.x < 0;   
    setState(SpriteState::INACTIVE);
}

void Rex::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();
    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::ACTIVE){
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

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

void Rex::activeWhenMarioApproach(Character& character) {
    Enemy::activeWhenMarioApproach(character);
}

void Rex::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}

void Rex::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}
