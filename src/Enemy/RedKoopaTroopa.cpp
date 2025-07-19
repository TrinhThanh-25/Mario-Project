#include "Enemy/RedKoopaTroopa.h"
#include "Common/ResourceManager.h"

RedKoopaTroopa::RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::RED_KOOPA_TROOPA, pos, dim, vel, color){
    setState(SpriteState::INACTIVE); 
    isFacingLeft = vel.x < 0;  
    type = EnemyType::RED_KOOPA_TROOPA; 
}
    
RedKoopaTroopa::~RedKoopaTroopa(){

}
    
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

    // Bay lên
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;

    // Lấp lánh bằng alpha modulate (sin wave)
        float alpha = 0.5f + 0.5f * sin(GetTime() * 10.0f); // dao động từ 0 → 1
        Color glowColor = {255, 255, 255, (unsigned char)(255 * alpha)};

        DrawTexture(
            ResourceManager::getTexture()["Gui100"],
            diePosition.x,
            diePosition.y - offsetY,
            glowColor
        );
    }
    north.setColor(BLUE);
    south.setColor(BLUE);
    west.setColor(BLUE);
    east.setColor(BLUE);
    north.draw();
    west.draw();
    south.draw();
    east.draw();
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
    
void RedKoopaTroopa::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
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

void RedKoopaTroopa::activeWhenMarioApproach(Character& character)
{
   Enemy::activeWhenMarioApproach(character);
}

void RedKoopaTroopa::followTheLeader(Sprite* leader) {
    if (!leader || state != SpriteState::ACTIVE) return;

    Vector2 leaderPos = leader->getPosition();
    float delta = GetFrameTime();

    // Nếu leader cách quá xa dhì chạy dheo
    if (fabs(position.x - leaderPos.x) > 32.0f) {
        isFacingLeft = leaderPos.x < position.x;
        velocity.x = isFacingLeft ? -40.0f : 40.0f; // tốc độ như trong update()

        position.x += velocity.x * delta;
        updateCollisionBoxes();
    }
}

void RedKoopaTroopa::collisionTile(Tile* tile) {
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

void RedKoopaTroopa::collisionBlock(Block* block) {
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

bool RedKoopaTroopa::isNearEdge() {
    return true;
    // float checkOffsetX = isFacingLeft ? -1.0f : dimension.x + 1.0f;
    // Vector2 checkPos = {
    //     position.x + checkOffsetX,
    //     position.y + dimension.y + 1.0f
    // };

    // return !world->getMap()->isSolidTileAt(checkPos);
}