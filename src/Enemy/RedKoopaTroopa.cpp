#include "Enemy/RedKoopaTroopa.h"
#include "Common/ResourceManager.h"

RedKoopaTroopa::RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color){
    setState(SpriteState::INACTIVE); 
    isFacingLeft = vel.x < 0;   
}
    
RedKoopaTroopa::~RedKoopaTroopa(){}

    
void RedKoopaTroopa::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        if (isFacingLeft) {
            textureKey = (frame == 0) ? "RedKoopaTroopa0Left" : "RedKoopaTroopa1Left";
        } else {
            textureKey = (frame == 0) ? "RedKoopaTroopa0Right" : "RedKoopaTroopa1Right";
        }

        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "RedKoopaTroopa1Left" : "RedKoopaTroopa1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        DrawTexture(ResourceManager::getTexture()["Point100"], diePosition.x, diePosition.y - offsetY, WHITE);
    }
}
 
    
void RedKoopaTroopa::beingHit(HitType type) {
    switch (type) {
        case HitType::STOMP:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::SHELL);
                velocity = {0, 0};
                shellMoving = false;
            }
            else if (state == SpriteState::SHELL) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                shellMoving = false;
            }
            break;

        case HitType::SHELL_KICK:
            if (state == SpriteState::SHELL) {
                shellMoving = true;
                isFacingLeft = !isFacingLeft; // đổi hướng nếu bị đá
            } 
            else if (state == SpriteState::ACTIVE) {
                // Bị mai trượt → chết luôn
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                velocity = {0, 0};
                shellMoving = false;
            }
            break;

        case HitType::FIREBALL:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                dyingFrameTime = 0.2f; 
                velocity = {0, 0};
                shellMoving = false;
            }
            break;

        default:
            break;
    }
}

   
void RedKoopaTroopa::collisionSound(){

}
    
void RedKoopaTroopa::update(Mario& mario, const std::vector<Sprite*>& collidables) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        activeWhenMarioApproach(mario);
        return;
    }

    if (state == SpriteState::ACTIVE) {

        if (leader) {
            followTheLeader(leader);
        }

        // Gravity
        velocity.y += 980.0f * delta;

        // Đổi hướng nếu gần mép (đặc trưng Red Koopa)
        if (isNearEdge()) {
            velocity.x = -velocity.x;
            isFacingLeft = !isFacingLeft;
        }

        // Di chuyển
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // Va chạm ngang => đổi hướng
        // CollisionType collision = checkCollision(collidables);
        // if (collision == CollisionType::WEST || collision == CollisionType::EAST) {
        //     velocity.x = -velocity.x;
        //     isFacingLeft = velocity.x < 0;
        // }

        // Cập nhật hướng nhìn
        if (velocity.x != 0) {
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


void RedKoopaTroopa::activeWhenMarioApproach(Mario& mario) {
    float distanceX = std::abs(mario.getPosition().x - position.x);
    if (distanceX < 300.0f) { // tuỳ chỉnh khoảng cách
        setState(SpriteState::ACTIVE);
    }
}

void RedKoopaTroopa::followTheLeader(Sprite* leader) {
    if (!leader || state != SpriteState::ACTIVE) return;

    Vector2 leaderPos = leader->getPosition();
    float delta = GetFrameTime();

    // Nếu leader cách quá xa thì chạy theo
    if (fabs(position.x - leaderPos.x) > 32.0f) {
        isFacingLeft = leaderPos.x < position.x;
        velocity.x = isFacingLeft ? -40.0f : 40.0f; // tốc độ như trong update()

        position.x += velocity.x * delta;
        updateCollisionBoxes();
    }
}



// bool RedKoopaTroopa::isNearEdge() {
//     float checkOffsetX = isFacingLeft ? -1.0f : dimension.x + 1.0f;
//     Vector2 checkPos = {
//         position.x + checkOffsetX,
//         position.y + dimension.y + 1.0f
//     };

//     return !world->getMap()->isSolidTileAt(checkPos);
// }


