#include "Enemy/BulletBill.h"
#include "Common/ResourceManager.h"

BulletBill::BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color)
{
    setState(SpriteState::ACTIVE);             // Luôn bắt đầu hoạt động
    isFacingLeft = vel.x < 0;                  // Xác định hướng bay
    velocity = vel;                            // Không thay đổi trong quá trình bay
}

    
void BulletBill::update(Mario& mario, const std::vector<Sprite*>& collidables){
    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE) {
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // if (checkCollision(collidables) != CollisionType::NONE) {
        //     setState(SpriteState::TO_BE_REMOVED);
        // }

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
    
void BulletBill::draw(){
    std::string textureKey;

    if (state == SpriteState::ACTIVE){
        textureKey = isFacingLeft ? "BulletBill0Left" : "BulletBill0Right";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}
    
void BulletBill::beingHit(HitType type){
    if (type == HitType::STOMP){
        return;
    }

    if (type == HitType::FIREBALL || type == HitType::SHELL_KICK){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
        }
    }
}
    
void BulletBill::activeWhenMarioApproach(Mario& mario) {

}