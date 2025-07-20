#include "Item/Item.h"
#include "Game/World.h"

Item::Item(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction, float HitFrameTime, int maxHitFrame, bool pause):
Sprite(position,size,vel,color,frameTime,maxFrame,direction), beingHitFrameTime(HitFrameTime), maxBeingHitFrame(maxHitFrame), pauseGameWhenHit(pause)
{
    this->setState(SpriteState::ACTIVE);
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
                character->getWorld()->pauseWorld(true, false);
            }
            this->updateCharacter(character);
        } 
        if (this->getY() > character->getMap()->getHeight()) {
            this->setState(SpriteState::TO_BE_REMOVED);
        }
    }
}

ItemType Item::getType()
{
    return type;
}

json Item::saveToJson() const
{
    json j = Sprite::saveToJson();
    j["beingHitFrameTime"] = beingHitFrameTime;
    j["beingHitFrameAcum"] = beingHitFrameAcum;
    j["maxBeingHitFrame"] = maxBeingHitFrame;
    j["currentBeingHitFrame"] = currentBeingHitFrame;
    j["pointFrameAccum"] = pointFrameAccum;
    j["pointFrameTime"] = pointFrameTime;
    j["pauseGameWhenHit"] = pauseGameWhenHit;
    j["type"] = (int)type;
    return j;
}

void Item::loadFromJson(const json& j)
{
    Sprite::loadFromJson(j);
    beingHitFrameTime = j.value("beingHitFrameTime", 0.0f);
    beingHitFrameAcum = j.value("beingHitFrameAcum", 0.0f);
    maxBeingHitFrame = j.value("maxBeingHitFrame", 0);
    currentBeingHitFrame = j.value("currentBeingHitFrame", 0);
    pointFrameAccum = j.value("pointFrameAccum", 0.0f);
    pointFrameTime = j.value("pointFrameTime", 0.0f);
    pauseGameWhenHit = j.value("pauseGameWhenHit", false);
    type = static_cast<ItemType>(j.value("type", static_cast<int>(ItemType::COIN)));
}
