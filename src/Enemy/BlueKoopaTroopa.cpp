#include "Enemy/BlueKoopaTroopa.h"
#include "Common/ResourceManager.h"
#include <string>

#define WAKE_UP_TIME 5.0f

BlueKoopaTroopa::BlueKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::BLUE_KOOPA_TROOPA, pos, dim, vel, color) {
    
    extraWakeUpTime = 2.0f;
    shellSpeed = 150.0f;
    shellTimer = 0.0f;
    shellMoving = false;

    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;
    type == EnemyType::BLUE_KOOPA_TROOPA;
}

void BlueKoopaTroopa::update(const std::vector<Character*>& characterList) {

        if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }
    
    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE) {
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;


        updateCollisionBoxes();
    }

    else if (state == SpriteState::SHELL) {
        velocity.y += 981.0f * delta;
        position.y += velocity.y * delta;


        shellTimer += delta;
        updateCollisionBoxes();

        if (shellTimer >= WAKE_UP_TIME + extraWakeUpTime) {
            setState(SpriteState::ACTIVE);
            velocity.x = isFacingLeft ? -40.0f : 40.0f;
            shellTimer = 0.0f;
        }
    }

    else if (state == SpriteState::SHELL_MOVING) {
        velocity.y += 981.0f * delta;
        float dir = isFacingLeft ? -1.0f : 1.0f;
        position.x += shellSpeed * dir * delta;
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

void BlueKoopaTroopa::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE) {
        textureKey = isFacingLeft ? (frame == 0 ? "BlueKoopaTroopa0Left" : "BlueKoopaTroopa1Left")
                                  : (frame == 0 ? "BlueKoopaTroopa0Right" : "BlueKoopaTroopa1Right");
    }

    // else if (state == SpriteState::SHELL || state == SpriteState::SHELL_MOVING) {
    // textureKey = isFacingLeft ? "BlueKoopaShellLeft" : "BlueKoopaShellRight";
    // }

    // Bạn có thể thêm shell / dying nếu có sprite riêng
    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "BlueKoopaTroopa1Left" : "BlueKoopaTroopa1Right";
        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        float angle = sin(GetTime() * 10.0f) * 10.0f;

        Texture2D& guiTex = ResourceManager::getTexture()["Gui100"];
        DrawTexturePro(
            guiTex,
            Rectangle{ 0, 0, (float)guiTex.width, (float)guiTex.height },
            Rectangle{
                diePosition.x,
                diePosition.y - offsetY,
                (float)guiTex.width,
                (float)guiTex.height
            },
            Vector2{ guiTex.width / 2.0f, guiTex.height / 2.0f },
            angle,
            WHITE
        );
    }
}


void BlueKoopaTroopa::beingHit(HitType type) {
    switch (type) {
        case HitType::STOMP:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::SHELL);
                velocity.x = 0;
                shellMoving = false;
                shellTimer = 0.0f;
            }
            else if (state == SpriteState::SHELL) {
                setState(SpriteState::SHELL_MOVING);
                shellMoving = true;
                shellTimer = 0.0f;
            }
            else if (state == SpriteState::SHELL_MOVING) {
                setState(SpriteState::SHELL);
                shellMoving = false;
                shellTimer = 0.0f;
            }
            break;

        case HitType::FIREBALL:
        case HitType::SHELL_KICK:
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
            break;

        default:
            break;
    }
}

void BlueKoopaTroopa::kickShell(bool faceLeft) {
    if (state == SpriteState::SHELL) {
        setState(SpriteState::SHELL_MOVING);
        shellMoving = true;
        isFacingLeft = faceLeft;
        shellTimer = 0.0f;
    }
}

bool BlueKoopaTroopa::isShellMoving() const {
    return shellMoving;
}

void BlueKoopaTroopa::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void BlueKoopaTroopa::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}

void BlueKoopaTroopa::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }

    if (col == CollisionType::SOUTH){
        velocity.y = 0;
    }
}
