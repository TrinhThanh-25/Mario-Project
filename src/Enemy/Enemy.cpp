#include "Enemy/Enemy.h"
#include <cmath>
#include <initializer_list>
#include <algorithm>

// ======================== CONSTRUCTOR / DESTRUCTOR ==========================
Enemy::Enemy(EnemyType enemyType, Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Sprite(pos, dim, vel, color, 0, 2, Direction::LEFT),
    type(enemyType),
    dyingFrameAcum(0.0f),
    dyingFrameTime(0.1f),
    maxDyingFrame(2),
    currentDyingFrame(0),
    pointFrameAcum(0.0f),
    pointFrameTime(0.1f),
    diePosition(pos)
{
    setState(SpriteState::ACTIVE);
}

Enemy::~Enemy(){}

void Enemy::update() {
    
}


// ======================== COLLISIONBOX ==========================
Rectangle Enemy::getCollisionBox(){
    return Rectangle{ position.x, position.y, size.x, size.y};
}


// CollisionType Enemy::checkCollision(Block* block);
void Enemy::collisionBlock(Block* block){
    switch (checkCollision(block))
    {
    case CollisionType::SOUTH:
        position.y = block->getY() - size.y;
        velocity.y = 0;
        updateCollisionBoxes();
        break;
    case CollisionType::NORTH:
        position.y = block->getY() + size.y;
        velocity.y = 0;
        updateCollisionBoxes();
        break;
    case CollisionType::EAST:
        position.x = block->getX() - size.x;
        velocity.x = 0;
        updateCollisionBoxes();
        break;
    case CollisionType::WEST:
        position.x = block->getX() + size.x;
        velocity.x = 0;
        updateCollisionBoxes();
        break;
    default:
        break;
    }
}

void Enemy::collisionTile(Tile* tile) {
    switch (checkCollision(tile)) {
        case CollisionType::SOUTH:
            position.y = tile->getY() - size.y;
            velocity.y = 0;
            updateCollisionBoxes();
            break;

        case CollisionType::WEST: 
            position.x = tile->getX() + tile->getWidth();
            velocity.x = 0;
            updateCollisionBoxes();
            break;

        case CollisionType::EAST: 
            position.x = tile->getX() - size.x;
            velocity.x = 0;
            updateCollisionBoxes();
            break;

        case CollisionType::NORTH: 
            position.y = tile->getY() + tile->getHeight();
            velocity.y = 0;
            updateCollisionBoxes();
            break;

        default:
            break;
    }
}


// ======================== MAIN METHOD ==========================



void Enemy::activeWhenMarioApproach(Character& character) {
    if (state != SpriteState::INACTIVE)
        return;

    float dx = std::abs(position.x - character.getPosition().x);
    if (dx <= 3200.0f) {
        setState(SpriteState::ACTIVE);
    }
}


void Enemy::beingHit(HitType type) {
    setState(SpriteState::DYING);
    currentDyingFrame = 0;
    dyingFrameAcum = 0.0f;
    diePosition = position;
}

void Enemy::followTheLeader(Sprite* leader){

}

// CollisionType Enemy::checkCollision(const std::vector<Sprite*>& collidables) {
//     for (Sprite* sprite : collidables) {
//         if (sprite == nullptr) continue;
//         CollisionType result = this->checkCollision(sprite);
//         if (result != CollisionType::NONE) {
//             return result;
//         }
//     }
//     return CollisionType::NONE;
// }

void Enemy::collisionSound(){

}

EnemyType Enemy::getEnemyType(){
    return type;
}

int Enemy::getPoint() const {
    return point;
}

// ======================== SAVE GAME ==========================


json Enemy::saveToJson() const {
    json j = Sprite::saveToJson();
    j["dyingFrameAcum"] = dyingFrameAcum;
    j["dyingFrameTime"] = dyingFrameTime;
    j["maxDyingFrame"] = maxDyingFrame;
    j["currentDyingFrame"] = currentDyingFrame;
    j["pointFrameAcum"] = pointFrameAcum;
    j["pointFrameTime"] = pointFrameTime;
    j["diePosition"] = {diePosition.x, diePosition.y};
    j["isFacingLeft"] = isFacingLeft;
    j["type"] = (int)type;
    return j;
}

void Enemy::loadFromJson(const json& j){
    Sprite::loadFromJson(j);
    dyingFrameAcum = j["dyingFrameAcum"].get<float>();
    dyingFrameTime = j["dyingFrameTime"].get<float>();
    maxDyingFrame = (int)j["maxDyingFrame"];
    currentDyingFrame = (int)j["currentDyingFrame"];
    pointFrameAcum = j["pointFrameAcum"].get<float>();
    pointFrameTime = j['pointFrameTime'].get<float>();
    diePosition = {
        j["diePosition"][0].get<float>(),
        j["diePosition"][1].get<float>()
    };
    isFacingLeft = j["isFacingLeft"].get<bool>();
    type = static_cast<EnemyType>(j["type"].get<int>());
}
