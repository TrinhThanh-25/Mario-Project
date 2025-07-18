#include "Enemy/BulletBill.h"
#include "Common/ResourceManager.h"

BulletBill::BulletBill(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(pos, dim, vel, color)
{
    setState(SpriteState::INACTIVE);             // Luôn bắt đầu hoạt động
    isFacingLeft = vel.x < 0;                  // Xác định hướng bay
    velocity = vel;                            // Không thay đổi trong quá trình bay
}

    
void BulletBill::update(const std::vector<Character*>& characterList){
    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }
    
    float delta = GetFrameTime();

    if (state == SpriteState::ACTIVE) {
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // if (checkCollision(collidables) != CollisionType::NONE) {
        //     setState(SpriteState::TO_BE_REMOVED);
        // }

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
    
void BulletBill::draw(){
    std::string textureKey;

    if (state == SpriteState::ACTIVE){
        textureKey = isFacingLeft ? "BulletBill0Left" : "BulletBill0Right";
    }

    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);

    if (state == SpriteState::DYING) {
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

    
void BulletBill::beingHit(HitType type){
    if (type == HitType::STOMP){
        return;
    }

    if (type == HitType::FIREBALL || type == HitType::SHELL_KICK){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            dyingFrameAcum = 0.0f;
            pointFrameAcum = 0.0f;
        }
    }
}
    
void BulletBill::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void BulletBill::collisionTile(Tile* tile) {
    CollisionType col = checkCollision(tile);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionTile(tile);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        setState(SpriteState::TO_BE_REMOVED);
    }
}

void BulletBill::collisionBlock(Block* block) {
    CollisionType col = checkCollision(block);

    // Gọi xử lý gốc để vẫn giữ va chạm đất/trần
    Enemy::collisionBlock(block);

    if (col == CollisionType::WEST || col == CollisionType::EAST) {
        setState(SpriteState::TO_BE_REMOVED);
    }
}