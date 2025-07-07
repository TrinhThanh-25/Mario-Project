#include "Enemy/FlyingGoomba.h"
#include "Common/ResourceManager.h"


FlyingGoomba::FlyingGoomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color){}


FlyingGoomba::~FlyingGoomba(){}

void FlyingGoomba::draw(){
    std::string textureKey;
    if (state == SpriteState::ACTIVE){
        int frame = (int)(GetFrameTime() * 6) % 2;
        if (movetype == MoveType::FLYING){
            if (isFacingLeft){
                textureKey = (frame == 0) ? "FlyingGoomba_0Left" : "FlyingGoomba_1Left";
            }
            else {
                textureKey = (frame == 0) ? "FlyingGoomba_0Right" : "FlyingGoomba_1Right";
            }
        }
        else if (movetype == MoveType::WALKING){
            if (isFacingLeft){
                textureKey = (frame == 0) ? "FlyingGoomba_2Left" : "FlyingGoomba_3Left";
            }
            else {
                textureKey = (frame == 0) ? "FlyingGoomba_2Right" : "FlyingGoomba_3Right";
            }
        }
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }
    else if (state == SpriteState::DYING){
        std::string dyingKey = isFacingLeft ? "FlyingGoomba_1Left" : "FlyingGoomba_1Right"; 
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }

}

void FlyingGoomba::update(Mario& mario){
    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE){
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        if (movetype == MoveType::FLYING){
            jumpTimer += delta;
            if (jumpTimer >= jumpInterval){
                velocity.y = jumpSpeed;
                jumpTimer = 0.0f;
            }
        }
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        updateCollisionBoxes();
    }
    
    else if (state == SpriteState::DYING){
        dyingFrameAcum += delta;
        if (dyingFrameAcum >= dyingFrameTime){
            currentDyingFrame++;
            dyingFrameAcum = 0.0f;

            if (currentDyingFrame >= maxDyingFrame){
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
        }

        else if (movetype == MoveType::WALKING){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            dyingFrameTime = 0.0f;
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

void FlyingGoomba::activeWhenMarioApproach(Mario& mario){
    float distance = std::abs(mario.getPosition().x - position.x);
    if (distance < 200.0f) {
        setState(SpriteState::ACTIVE);
        setMoveType(MoveType::FLYING);
    }
}