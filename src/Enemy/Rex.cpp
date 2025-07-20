#include "Enemy/Rex.h"
#include "Common/ResourceManager.h"
#include <cmath>
#include <string>

#define ACTIVATION_RANGE 300.0f

Rex::Rex(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::REX, pos, dim, vel, color), isShrunken(false), shrinkDuration(0.0f) {
    isFacingLeft = vel.x < 0;   
    setState(SpriteState::INACTIVE);
    type = EnemyType::REX;
    point = 200;
}

void Rex::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();
    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }

    if (state == SpriteState::ACTIVE){
        velocity.y += World::gravity * delta;
        position.x += velocity.x * delta;
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
void Rex::draw() {
    std::string textureKey;
    int frame = (int) (GetTime() * 6) % 2;

    if (state == SpriteState::ACTIVE){
        if (isShrunken){
            if (isFacingLeft){
                textureKey = (frame == 0) ? "Rex10Left" : "Rex11Left";
            }
            else {
                textureKey = (frame == 0) ? "Rex10Right" : "Rex11Right";
            }
        }
        else {
            if (isFacingLeft){
                textureKey = (frame == 0) ? "Rex20Left" : "Rex21Left";
            }
            else {
                textureKey = (frame == 0) ? "Rex20Right" : "Rex21Right";
            }
        }
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);     
    }

    if (state == SpriteState::DYING) {
        std::string dyingKey = isFacingLeft ? "Rex10Left" : "Rex10Right";

        DrawTexture(ResourceManager::getTexture()[dyingKey], position.x, position.y, WHITE);

        float offsetY = 50.0f * pointFrameAcum / pointFrameTime;
        float angle = sin(GetTime() * 10.0f) * 10.0f;

        Texture2D& guiTex = ResourceManager::getTexture()["Gui100"];
        DrawTexturePro(
            guiTex,
            Rectangle{0, 0, (float)guiTex.width, (float)guiTex.height},
            Rectangle{
                diePosition.x,
                diePosition.y - offsetY,
                (float)guiTex.width,
                (float)guiTex.height
            },
            Vector2{guiTex.width / 2.0f, guiTex.height / 2.0f},
            angle,
            WHITE
        );
    }
}


void Rex::beingHit(HitType type) {
    if (type == HitType::STOMP){
        if (!isShrunken){
            isShrunken = true;
            velocity.x = 0;
            setSize({32, 32});
            updateCollisionBoxes();
        }
        else {
            setState(SpriteState::DYING);
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            diePosition = position;
        }
    }
    else if (type == HitType::FIREBALL || type == HitType::SHELL_KICK){
        setState(SpriteState::DYING);
        currentDyingFrame = 0;
        dyingFrameAcum = 0.0f;
        diePosition = position;
    }
}

void Rex::activeWhenMarioApproach(Character& character) {
    Enemy::activeWhenMarioApproach(character);
}

void Rex::collisionTile(Tile* tile) {
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

void Rex::collisionBlock(Block* block) {
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

// =============================== SAVE GAME =================================

json Rex::saveToJson() const {
    json j = Enemy::saveToJson();

    j["isShrunken"] = isShrunken;
    j["shrinkDuration"] = shrinkDuration;

    return j;
}

void Rex::loadFromJson(const json& j) {
    Enemy::loadFromJson(j);

    isShrunken = j["isShrunken"].get<bool>();
    shrinkDuration = j["shrinkDuration"].get<float>();
}
