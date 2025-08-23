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

    point = 200;
}

void BobOmb::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  
        }
        if (state == SpriteState::INACTIVE) return; 
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
        for (Character* c : characterList) {
            float dx = c->getPosition().x + c->getWidth() / 2 - (position.x + getWidth() / 2);
            float dy = c->getPosition().y + c->getHeight() / 2 - (position.y + getHeight() / 2);
            float distance = sqrt(dx * dx + dy * dy);

            if (distance <= explosionRadius) {
                c->setDying();
            }
            explosionFrameAcum += delta;
            if (explosionFrameAcum >= explosionFrameTime) {
                if(currentExplosionFrame < maxExplosionFrame) {
                    currentExplosionFrame++;
                } else {
                    setState(SpriteState::TO_BE_REMOVED);
                }
                explosionFrameAcum = 0.0f;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                diePosition = position;
            }
        }
        return;
    }

    if (state == SpriteState::ACTIVE) {
        switch (bobombState) {
            case BobOmbState::IDLE:
                position.x += velocity.x * delta;
                position.y += velocity.y * delta;
                velocity.y += World::gravity * delta;
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

    else if (state == SpriteState::EXPLODING) {
        DrawTexture(ResourceManager::getTexture()["Explode" + std::to_string(currentExplosionFrame)], position.x, position.y, WHITE);
    }
}


void BobOmb::beingHit(HitType type){
    if (bobombState == BobOmbState::IDLE) {
        bobombState = BobOmbState::IGNITED;
        ignitionTimer = 0.0f;
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
    if (!isAlive() || !isSolidTile(tile)) return;
    CollisionType col = checkCollision(tile);

    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE){
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
}

void BobOmb::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
}

json BobOmb::saveToJson() const {
    json j = Enemy::saveToJson();  

    j["isIgnited"] = isIgnited;
    j["ignitionTimer"] = ignitionTimer;
    j["maxIgniteTime"] = maxIgniteTime;
    j["explosionRadius"] = explosionRadius;
    j["bobombState"] = static_cast<int>(bobombState); 

    return j;
}

void BobOmb::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);  

    isIgnited = j["isIgnited"].get<bool>();
    ignitionTimer = j["ignitionTimer"].get<float>();
    maxIgniteTime = j["maxIgniteTime"].get<float>();
    explosionRadius = j["explosionRadius"].get<float>();
    bobombState = static_cast<BobOmbState>(j["bobombState"].get<int>());  
}
