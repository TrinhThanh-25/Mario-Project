#include "Enemy/MontyMole.h"
#include "Common/ResourceManager.h"


MontyMole::MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color){

    emergeDelay = 2.0f + (rand() % 1000) / 1000.0f; // Random 2.0 ~ 3.0s
    emergeTimer = 0;
    hasEmerge = false;
    }
    
MontyMole::~MontyMole(){}

void MontyMole::draw(){
    std::string textureKey;

    if (state == SpriteState::ACTIVE){
        int frame = (int)(GetTime() * 6) % 2;
        if (isFacingLeft){
            textureKey = (frame == 0) ? "MontyMole0Left" : "MontyMole1Left";
        }
        else {
            textureKey = (frame == 0) ? "MontyMole0Right" : "MontyMole1Right";
        }
    }
    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING){
        float offsetY = 50.0f * pointFrameAcum/pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);

    }
}

void MontyMole::update(){
    float delta = GetFrameTime();
    if (state == SpriteState::ACTIVE){
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        updateCollisionBoxes();
    }

    else if (state == SpriteState::DYING){
        dyingFrameAcum += delta;
        //position.y += gravity * delta;
        if (dyingFrameAcum >= dyingFrameTime){
            dyingFrameAcum = 0.0f;
            currentDyingFrame++;

            if (currentDyingFrame >= maxDyingFrame){
                setState(SpriteState::TO_BE_REMOVED);
            }
        }
        pointFrameAcum += delta;
        if (pointFrameAcum >= pointFrameTime){
            pointFrameAcum = pointFrameTime;
        }
    }
}

void MontyMole::beingHit(HitType type) {
    switch (type) {
        case HitType::STOMP:
        case HitType::FIREBALL:
        case HitType::SHELL_KICK:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                velocity.x = 0;
                velocity.y = 0;
            }
            break;

        default:
            // Các kiểu tấn công khác (nếu có) xử lý sau
            break;
    }
    // PlaySound(ResourceManager::getSound()["EnemyDefeat"]);
}


void MontyMole::collisionSound(){

}