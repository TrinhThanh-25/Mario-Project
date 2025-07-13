#include "Enemy/BobOmb.h"
#include "Common/ResourceManager.h"

#include "Enemy/BobOmb.h"

BobOmb::BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color) {
    
    setState(SpriteState::ACTIVE);       // BobOmb luôn hoạt động ngay khi xuất hiện

    isIgnited = false;                   // Chưa bị kích nổ
    ignitionTimer = 0.0f;
    maxIgniteTime = 2.5f;                // Thời gian chờ nổ sau khi bị kích hoạt
    explosionRadius = 48.0f;             // Phạm vi nổ (có thể chỉnh tùy gameplay)

    isFacingLeft = vel.x < 0;            // Hướng ban đầu dựa theo velocity
}

void BobOmb::update(Mario& mario, const std::vector<Sprite*>& collidables){
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE){
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE){
        if (!isIgnited){
            velocity.y += 981.0f * delta;
            position.x += velocity.x * delta;
            position.y += velocity.y * delta;

            if (checkCollision(collidables) != CollisionType::NONE) {
                velocity.x = -velocity.x;
                isFacingLeft = velocity.x < 0;
            }

            updateCollisionBoxes();
        }

        else {
            velocity = {0.0f, 0.0f}; // ✅
            ignitionTimer += delta;
            if (ignitionTimer >= maxIgniteTime){
                setState(SpriteState::EXPLODING);

            }
        }

        if (state == SpriteState::EXPLODING) {
        // Cần có texture nổ: vẽ animation hoặc chờ thời gian rồi remove
            setState(SpriteState::TO_BE_REMOVED);
            return;
        }


    }

}

void BobOmb::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "BobOmb0Left" : "BobOmb1Left")
                                  : (frame == 0 ? "BobOmb0Right" : "BobOmb1Right");
    } 

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {

        std::string dyingKey = isFacingLeft ? "BobOmb0Left" : "BobOmb0Right"; 
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }

    // else if (state == SpriteState::EXPLODING) {
    //     DrawTexture(ResourceManager::getTexture()["BobOmbExplode"], position.x, position.y, WHITE);
    // }

}

void BobOmb::beingHit(HitType type){
    if (type == HitType::STOMP || type == HitType::SHELL_KICK){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
        }
    }

    else if (type == HitType::FIREBALL){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::EXPLODING);
            isIgnited = true;
        }
    }
}
void BobOmb::activeWhenMarioApproach(Mario& mario){
    if (state != SpriteState::INACTIVE) return;

    float dx = abs(mario.getPosition().x - position.x);
    setState(SpriteState::ACTIVE);
    if (dx <= 40.0f && !isIgnited){
        isIgnited = true;
        ignitionTimer = 0.0f;
        velocity = {0, 0};
    }
}