#include "Common/ResourceManager.h"
#include "Enemy/Goomba.h" 


    
Goomba::Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color){
    setState(SpriteState::ACTIVE);
}
    
Goomba::~Goomba(){}


void Goomba::draw(){
    std::string textureKey;

    if (state == SpriteState::ACTIVE){
        int frame = (int)(GetTime() * 6) % 2; 
        if (isFacingLeft){
            textureKey = (frame == 0) ? "Goomba0Left" : "Goomba1Left";
        }
        else {
            textureKey = (frame == 0) ? "Goomba0Right" : "Goomba1Right";
        }
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }

    else if (state == SpriteState::DYING){
        std::string dyingKey = isFacingLeft ? "Goomba1Left" : "Goomba1Right"; 
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}

void Goomba::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE) {
        // Cập nhật hướng nhìn
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        //  Gravity
        velocity.y += 981.0f * delta;

        // Di chuyển
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // Va chạm với map
        CollisionType collision = checkCollision(collidables);
        if (collision == CollisionType::WEST || collision == CollisionType::EAST) {
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


void Goomba::beingHit(HitType type){
    if (type == HitType::STOMP){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            dyingFrameTime = 0.2f; 
            velocity = {0, 0};
        }
    }
    else { // them hieu ung sau
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            dyingFrameTime = 0.2f; 
            velocity = {0, 0};
        }
    }
}

void Goomba::collisionSound(){

}
    
void Goomba::activeWhenMarioApproach(Mario& mario){}