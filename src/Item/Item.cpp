#include "Item/Item.h"
#include "Item.h"

Item::Item() {}

Item::Item(Vector2 position, Vector2 size, Color color): Sprite(position,size,color),beingHitFrameTime(0), 
beingHitFrameAcum(0), maxBeingHitFrame(0) , currentBeingHitFrame(0), pointFrameAccum(0), pointFrameTime(0), pauseGameWhenHit(false)
{
}

Item::Item(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction, float HitFrameTime, int maxHitFrame, bool pause):
Sprite(position,size,color,vel,frameTime,maxFrame,direction), beingHitFrameTime(HitFrameTime), maxBeingHitFrame(maxHitFrame), pauseGameWhenHit(pause)
{
}

Item::~Item()
{
    Sprite::~Sprite();
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
    case WEST:
        this->setX(block->getX() + block->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case EAST:
        this->setX(block->getX() - this->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case NORTH:
        break;
    case SOUTH:
        this->setVelocityY(0);
        this->setY(block->getY() - this->getHeight());
        //collisionSouth(World::getCharacters());
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
    case WEST:
        this->setX(tile->getX() + tile->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case EAST:
        this->setX(tile->getX() - this->getWidth());
        this->setVelocityX(-this->getVelocityX());
        updateCollisionBoxes();
        break;
    case NORTH:
        break;
    case SOUTH:
        this->setVelocityY(0);
        this->setY(tile->getY() - this->getHeight());
        //collisionSouth
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
        /*Fall out of the map
        if (this->getY() > character->getWorld()->getMap()->getHeight()) {
            this->setState(SpriteState::TO_BE_REMOVED);
        }*/
    }
}
