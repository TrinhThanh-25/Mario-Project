#include "Enemy/BobOmb.h"
#include "Common/ResourceManager.h"

#include "Enemy/BobOmb.h"

BobOmb::BobOmb(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::BOB_OMB, pos, dim, vel, color) {
    
    setState(SpriteState::INACTIVE);
    bobombState = BobOmbState::IDLE;       // BobOmb luôn hoạt động ngay khi xuất hiện

    isIgnited = false;                   // Chưa bị kích nổ
    ignitionTimer = 0.0f;
    maxIgniteTime = 2.5f;                // Thời gian chờ nổ sau khi bị kích hoạt
    explosionRadius = 48.0f;             // Phạm vi nổ (có thể chỉnh tùy gameplay)

    isFacingLeft = vel.x < 0;            // Hướng ban đầu dựa theo velocity
    type = EnemyType::BOB_OMB;
}

void BobOmb::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::DYING) {
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
        return;
    }

    if (state == SpriteState::EXPLODING) {
        // Có thể thêm hiệu ứng nổ nếu cần
        setState(SpriteState::TO_BE_REMOVED);
        return;
    }

    if (state == SpriteState::ACTIVE) {
        switch (bobombState) {
            case BobOmbState::IDLE:
                velocity.y += 981.0f * delta;
                position.x += velocity.x * delta;
                position.y += velocity.y * delta;
                updateCollisionBoxes();
                break;

            case BobOmbState::IGNITED:
                velocity = {0, 0};
                ignitionTimer += delta;
                if (ignitionTimer >= maxIgniteTime) {
                    bobombState = BobOmbState::EXPLODED;
                    setState(SpriteState::EXPLODING);
                }
                break;

            default:
                break;
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

    // else if (state == SpriteState::EXPLODING) {
    //     DrawTexture(ResourceManager::getTexture()["BobOmbExplode"], position.x, position.y, WHITE);
    // }
}


void BobOmb::beingHit(HitType type){
    if (type == HitType::STOMP || type == HitType::SHELL_KICK){
        if (bobombState == BobOmbState::IDLE) {
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
        }
    }
    else if (type == HitType::FIREBALL){
        if (bobombState == BobOmbState::IDLE) {
            bobombState = BobOmbState::IGNITED;
            ignitionTimer = 0.0f;
        }
    }
}

void BobOmb::activeWhenMarioApproach(Character& character){
    if (state != SpriteState::INACTIVE) return;

    float dx = std::abs(character.getPosition().x - position.x);
    if (dx <= 3200.0f) {
        setState(SpriteState::ACTIVE);
        return;
    }

    if (dx <= 40.0f && bobombState == BobOmbState::IDLE){
        bobombState = BobOmbState::IGNITED;
        ignitionTimer = 0.0f;
        velocity = {0, 0};
    }
}


void BobOmb::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }
}

void BobOmb::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        velocity.x = -velocity.x;
        isFacingLeft = velocity.x < 0;
    }
}