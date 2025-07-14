#include "Item/Item.h"
#include "Game/World.h"
float Item::gravity = 1200.0f;

Item::Item(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction, float HitFrameTime, int maxHitFrame, bool pause):
Sprite(position,size,vel,color,frameTime,maxFrame,direction), beingHitFrameTime(HitFrameTime), maxBeingHitFrame(maxHitFrame), pauseGameWhenHit(pause)
{
}

Item::~Item()
{
    Sprite::~Sprite();
}

void Item::update()
{
}
void Item::draw()
{
}
bool Item::isPausedGameWhenBeingHit()
{
    return pauseGameWhenHit;
}

void Item::collisionBlock(Block *block)
{
    CollisionType type = checkCollision(block);
    switch (type)
    {
    case CollisionType::WEST:
        this->setX(block->getX() + block->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case CollisionType::EAST:
        this->setX(block->getX() - this->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case CollisionType::NORTH:
        break;
    case CollisionType::SOUTH:
        this->setVelocityY(0);
        this->setY(block->getY() - this->getHeight());
        collisionSouth(nullptr);
        updateCollisionBoxes();
        break;
    default:
        break;
    }
}

void Item::collisionTile(Tile* tile)
{
    CollisionType type = checkCollision(tile);
    switch (type)
    {
    case CollisionType::WEST:
        this->setX(tile->getX() + tile->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case CollisionType::EAST:
        this->setX(tile->getX() - this->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case CollisionType::NORTH:
        break;
    case CollisionType::SOUTH:
        this->setVelocityY(0);
        this->setY(tile->getY() - this->getHeight());
        collisionSouth(nullptr);
        updateCollisionBoxes();
        break;
    default:
        break;
    }
}

void Item::collisionCharacter(Character *character)
{
    if (this->getState() != SpriteState::TO_BE_REMOVED && this->getState() != SpriteState::HIT) {
        if (checkCollision(character) != CollisionType::NONE) {
            this->setState(SpriteState::HIT);
            this->playCollisionSound();
            if (this->isPausedGameWhenBeingHit()) {
                // Pause the game
            }
            this->updateCharacter(character);
        } 
        if (this->getY() > character->getMap()->getHeight()) {
            this->setState(SpriteState::TO_BE_REMOVED);
        }
    }
}
