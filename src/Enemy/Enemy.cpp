#include "Enemy/Enemy.h"
#include <cmath>
#include <initializer_list>
#include <algorithm>

// ======================== CONSTRUCTOR / DESTRUCTOR ==========================
Enemy::Enemy(Vector2 pos, Vector2 dim, Vector2 vel, Color color)
    : Sprite(pos, dim, vel, color, 0, 2, Direction::LEFT),
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
        setState(SpriteState::ON_GROUND);
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
            setState(SpriteState::ON_GROUND);
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
void Enemy::update() {
    float delta = GetFrameTime();

    if (state == SpriteState::DYING) {
        dyingFrameAcum += delta;

        if (dyingFrameAcum >= dyingFrameTime) {
            currentDyingFrame++;
            dyingFrameAcum = 0.0f;
        }

        if (currentDyingFrame >= maxDyingFrame) {
            setState(SpriteState::TO_BE_REMOVED);
        }
    }
    else {
        position.x += velocity.x * delta;
        position.y += velocity.y * delta;

        // Cập nhật hướng
        if (velocity.x != 0) {
            isFacingLeft = velocity.x < 0;
        }

        updateCollisionBoxes();
    }
}



void Enemy::activeWhenMarioApproach(Mario& mario){}

void Enemy::beingHit(){
    setState(SpriteState::DYING);
    currentDyingFrame = 0;
    dyingFrameAcum = 0.0f;
    diePosition = position;
}

void Enemy::followTheLeader(Sprite* leader){

}

// void Enemy::collisionSound(){

// }

