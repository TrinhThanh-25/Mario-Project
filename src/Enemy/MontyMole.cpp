#include "Enemy/MontyMole.h"
#include "Common/ResourceManager.h"


MontyMole::MontyMole(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::MONTY_MOLE, pos, dim, vel, color){

    emergeDelay = 2.0f + (rand() % 1000) / 1000.0f; // Random 2.0 ~ 3.0s
    emergeTimer = 0;
    hasEmerge = false;
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;   
}

MontyMole::~MontyMole() {
    // Destructor logic if needed
}

void MontyMole::draw() {
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE && hasEmerge) {
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




void MontyMole::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);

            if (state == SpriteState::ACTIVE && !hasEmerge) {
                emergeTimer = 0.0f;
                break;
            }
        }
        if (state == SpriteState::INACTIVE) return;
    }


    if (state == SpriteState::ACTIVE && !hasEmerge) {
        emergeTimer += delta;
        if (emergeTimer >= emergeDelay) {
            hasEmerge = true;
            // Gán vận tốc sau khi emerge
            velocity.x = isFacingLeft ? -30.0f : 30.0f;
            velocity.y = 0.0f;
        }
        return; // chưa emerge thì chưa di chuyển
    }

    if (state == SpriteState::ACTIVE && hasEmerge) {
        velocity.y += 981.0f * delta;
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

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


void MontyMole::beingHit(HitType type) {
    if (!hasEmerge) return; // Không thể bị đánh khi chưa lòi lên

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
                velocity = {0, 0};
            }
            break;
        default:
            break;
    }
}


void MontyMole::activeWhenMarioApproach(Character& character) {
    Enemy::activeWhenMarioApproach(character);
}

void MontyMole::collisionSound(){

}

void MontyMole::collisionTile(Tile* tile) {
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

void MontyMole::collisionBlock(Block* block) {
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