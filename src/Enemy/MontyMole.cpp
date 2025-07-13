#include "Enemy/MontyMole.h"
#include "Common/ResourceManager.h"


MontyMole::MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color){

    emergeDelay = 2.0f + (rand() % 1000) / 1000.0f; // Random 2.0 ~ 3.0s
    emergeTimer = 0;
    hasEmerge = false;
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;   
}
    
MontyMole::~MontyMole(){}

void MontyMole::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        if (isFacingLeft) {
            textureKey = (frame == 0) ? "MontyMole0Left" : "MontyMole1Left";
        } else {
            textureKey = (frame == 0) ? "MontyMole0Right" : "MontyMole1Right";
        }

        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "MontyMole1Left" : "MontyMole1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}


void MontyMole::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();

    // Nếu chưa active, kiểm tra khoảng cách với Mario
    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE) {

        // Di chuyển
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // Cập nhật hướng nhìn
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        // Va chạm với map
        // CollisionType collision = checkCollision(collidables);
        // if (collision == CollisionType::WEST || collision == CollisionType::EAST) {
        //     velocity.x = -velocity.x;
        //     isFacingLeft = velocity.x < 0;
        // }

        updateCollisionBoxes();
    }

    // Đang bị tiêu diệt
    else if (state == SpriteState::DYING) {
        dyingFrameAcum += delta;
        if (dyingFrameAcum >= dyingFrameTime) {
            dyingFrameAcum = 0.0f;
            currentDyingFrame++;
            if (currentDyingFrame >= maxDyingFrame) {
                setState(SpriteState::TO_BE_REMOVED);
            }
        }

        // Hiệu ứng point bay lên
        pointFrameAcum += delta;
        if (pointFrameAcum >= pointFrameTime) {
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

void MontyMole::activeWhenMarioApproach(Mario& mario) {
    float distance = std::abs(mario.getPosition().x - position.x);
    if (distance < 200.0f) {
        setState(SpriteState::ACTIVE);
    }
}



void MontyMole::collisionSound(){

}