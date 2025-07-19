#include "Enemy/Muncher.h"
#include "Common/ResourceManager.h"

Muncher::Muncher(Vector2 pos, Vector2 dim, Color color)
    : Enemy(EnemyType::MUNCHER, pos, dim, {0.0}, color){
    setState(SpriteState::INACTIVE); 
    type = EnemyType::MUNCHER;
}

    
void Muncher::update(const std::vector<Character*>& characterList){
    if (state == SpriteState::INACTIVE) {
        for (Character* c : characterList) {
            activeWhenMarioApproach(*c);
            if (state != SpriteState::INACTIVE) break;  // Đã được kích hoạt thì dừng
        }
        if (state == SpriteState::INACTIVE) return; // Vẫn chưa được kích hoạt thì không làm gì
    }
}
    
void Muncher::draw(){
    std::string textureKey;
    int frame = (int)(GetTime() * 6) % 2;
    textureKey = (frame == 0) ? "Muncher0" : "Muncher1";
    DrawTexture(ResourceManager::getTexture()[textureKey], position.x, position.y, WHITE);
}
    
void Muncher::beingHit(HitType type){
    // không bị đánh
}
    
void Muncher::activeWhenMarioApproach(Character& character){
    Enemy::activeWhenMarioApproach(character);
}

void Muncher::collisionTile(Tile* tile) {
    Enemy::collisionTile(tile);
}

void Muncher::collisionBlock(Block* block) {
    Enemy::collisionBlock(block);
}