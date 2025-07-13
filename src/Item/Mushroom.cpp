#include "Item/Mushroom.h"

Mushroom::Mushroom(Vector2 position, Vector2 size, Color color, int points, bool isInInventory):
Item(position, size, {320, 0}, color, 0, 0, Direction::RIGHT, 0, 0, false), points(points), isInInventory(isInInventory)
{
    pauseGameWhenHit = true;
    if (isInInventory) {
        setVelocityY(100);
    }
}

void Mushroom::update()
{
    float timeElapsed = GetFrameTime();
    if (getState() == SpriteState::ACTIVE) {
        updateWhenActive(timeElapsed);
    } else if (getState() == SpriteState::HIT) {
        updateWhenHit(timeElapsed);
    } 
    updateCollisionBoxes();
}

void Mushroom::updateWhenActive(float timeElapsed)
{
    // Falling from the inventory
    if (isInInventory) {
        //Position
        setY(getY() + getVelocityY() * timeElapsed);

        //Blinking effect
        blinkFrameAccum += timeElapsed;
        if (blinkFrameAccum >= blinkFrameTime) {
            blinkFrameAccum -= blinkFrameTime;
            appear = !appear; // Toggle appearance
        }
    } else {
        setVelocityY(getVelocityY() + gravity * timeElapsed);
        setY(getY() + getVelocityY() * timeElapsed);
        if (getDirection() == Direction::RIGHT)
        {
            setX(getX() + getVelocityX() * timeElapsed);
        }
        else if (getDirection() == Direction::LEFT)
        {
            setX(getX() - getVelocityX() * timeElapsed);
        }
    }
}

void Mushroom::updateWhenHit(float timeElapsed)
{
    beingHitFrameAcum += timeElapsed;
    if (beingHitFrameAcum >= beingHitFrameTime)
    {
        beingHitFrameAcum -= beingHitFrameTime;
        currentBeingHitFrame++;
        if (currentBeingHitFrame >= maxBeingHitFrame)
        {
            this->setState(SpriteState::TO_BE_REMOVED);
        }
    }
    pointFrameAccum += timeElapsed;
    if (pointFrameAccum >= pointFrameTime) pointFrameAccum = pointFrameTime;
}

void Mushroom::draw()
{
    if (getState() == SpriteState::ACTIVE)
    {
        if (isInInventory && !appear) {
            // Do not draw if in inventory and not appearing
            return;
        }
        DrawTexture(ResourceManager::getTexture()["Mushroom"], getX(), getY(), getColor());
    }
    else if (getState() == SpriteState::HIT)
    {
        // Draw point floating above the mushroom
    }
}

void Mushroom::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["PowerUp"]);
}

void Mushroom::updateCharacter(Character *character)
{

    /*Add point here*/
    
    CharacterType type = character->getType();
    if (type == CharacterType::SMALL) {
        character->setY(character->getY() - 16); 
        character->setPreviousState(character->getState());
        character->setState(SpriteState::SMALL_TO_SUPER);
    } else if (type == CharacterType::SUPER || type == CharacterType::FLOWER) {
        // CharacterType previousType = character->getPreviousType();
        // if (previousType == CharacterType::SMALL) {
        //     PlaySound(ResourceManager::getSound()["StorePowerUpItem"]);
        //     /*Put mushroom into the inventory*/
        // }
    }
}

void Mushroom::collisionSouth(Character *character)
{
    if (isInInventory) {
        setDirection(character->getDirection());
        isInInventory = false;
    }
}
