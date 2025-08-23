#include "Enemy/RedKoopaTroopa.h"
#include "Common/ResourceManager.h"

RedKoopaTroopa::RedKoopaTroopa(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::RED_KOOPA_TROOPA, pos, dim, vel, color){
    setState(SpriteState::INACTIVE); 
    isFacingLeft = vel.x < 0;  
    type = EnemyType::RED_KOOPA_TROOPA; 
    point = 100;
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
        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;

        float alpha = 0.5f + 0.5f * sin(GetTime() * 10.0f); 
        Color glowColor = {255, 255, 255, (unsigned char)(255 * alpha)};

        DrawTexture(
            ResourceManager::getTexture()["Gui100"],
            diePosition.x,
            diePosition.y - offsetY,
            glowColor
        );
    }
}
 
    
void RedKoopaTroopa::beingHit(HitType type) {
    switch (type) {
        case HitType::STOMP:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                dyingFrameTime = 0.2f;
                shellMoving = false;
            }
            break;

        case HitType::SHELL_KICK:
            if (state == SpriteState::ACTIVE) {
                setState(SpriteState::DYING);
                diePosition = position;
                currentDyingFrame = 0;
                dyingFrameAcum = 0.0f;
                pointFrameAcum = 0.0f;
                dyingFrameTime = 0.2f;
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
            if (state != SpriteState::INACTIVE) break;  
        }
        if (state == SpriteState::INACTIVE) return; 
    }

    if (state == SpriteState::ACTIVE) {

        if (leader) {
            followTheLeader(leader);
        }

        position.x += velocity.x * delta;
        position.y += velocity.y * delta;
        velocity.y += World::gravity * delta;

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

    if (fabs(position.x - leaderPos.x) > 32.0f) {
        isFacingLeft = leaderPos.x < position.x;
        velocity.x = isFacingLeft ? -40.0f : 40.0f; // tốc độ như trong update()

        position.x += velocity.x * delta;
        updateCollisionBoxes();
    }
}

void RedKoopaTroopa::collisionTile(Tile* tile) {
    if (!isAlive() || !isSolidTile(tile)) return;
    CollisionType col = checkCollision(tile);
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

void RedKoopaTroopa::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        isFacingLeft = !isFacingLeft;
        if (state == SpriteState::ACTIVE) {
            velocity.x = isFacingLeft ? -100.0f : 100.0f;
        }
    }
    if (col == CollisionType::SOUTH) {
        velocity.y = 0;
    }
}

// =========================== SAVE GAME ====================
json RedKoopaTroopa::saveToJson() const {
    json j = Enemy::saveToJson();

    j["shellMoving"] = shellMoving;
    j["shellSpeed"] = shellSpeed;

    return j;
}

void RedKoopaTroopa::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    shellMoving = j["shellMoving"].get<bool>();
    shellSpeed = j["shellSpeed"].get<float>();
}

