#include "Common/ResourceManager.h"
#include "Enemy/Goomba.h" 


    
Goomba::Goomba(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Enemy(EnemyType::GOOMBA, pos, dim, vel, color){
    setState(SpriteState::INACTIVE);
    isFacingLeft = vel.x < 0;  
    type = EnemyType::GOOMBA; 
}

Goomba::~Goomba() {
    // Destructor logic if needed
}

void Goomba::draw(){
    std::string textureKey;

    if (state == SpriteState::ACTIVE){
        int frame = (int)(GetTime() * 6) % 2; 
        if (isFacingLeft){
            textureKey = (frame == 0) ? "Goomba0Left" : "Goomba1Left";
        } else {
            textureKey = (frame == 0) ? "Goomba0Right" : "Goomba1Right";
        }
        DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
    }

    else if (state == SpriteState::DYING){
        std::string dyingKey = isFacingLeft ? "Goomba1Left" : "Goomba1Right"; 
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


void Goomba::update(const std::vector<Character*>& characterList) {
    float delta = GetFrameTime();

    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }   

    if (state == SpriteState::ACTIVE) {
        // Cập nhật hướng nhìn
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        //  Gravity
        velocity.y += 981.0f * delta;

        // Di chuyển
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


void Goomba::beingHit(HitType type){
    if (type == HitType::STOMP){
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            dyingFrameTime = 0.2f; 
            velocity = {0, 0};
        }
    }
    else { // them hieu ung sau
        if (state == SpriteState::ACTIVE){
            setState(SpriteState::DYING);
            diePosition = position;
            currentDyingFrame = 0;
            dyingFrameAcum = 0.0f;
            dyingFrameTime = 0.2f; 
            velocity = {0, 0};
        }
    }
}

void Goomba::collisionSound(){

}
    
void Goomba::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void Goomba::collisionTile(Tile* tile) {
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

void Goomba::collisionBlock(Block* block) {
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