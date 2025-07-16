#include "Enemy/Muncher.h"
#include "Common/ResourceManager.h"

Muncher::Muncher(Vector2 pos, Vector2 dim, Color color)
    : Enemy(pos, dim, {0.0}, color){
    setState(SpriteState::ACTIVE); 
}

    
void Muncher::update(Mario& mario, const std::vector<Sprite*>& collidables){
 // không di chuyển
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
    
void Muncher::activeWhenMarioApproach(Mario& mario){
 // luôn active
}