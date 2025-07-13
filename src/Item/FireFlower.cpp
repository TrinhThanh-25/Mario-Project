#include "Item/FireFlower.h"

FireFlower::FireFlower(Vector2 position, Vector2 size, Color color, int points, bool isInInventory):
Item(position, size, Vector2(0, 0), color, 0.1f, 2, Direction::RIGHT, 0, 0, false), points(points), isInInventory(isInInventory)
{
    pauseGameWhenHit = true;
    if (isInInventory) {
        setVelocityY(100);
    }
}
void FireFlower::update()
{
    if (getState() == SpriteState::ACTIVE) {
        updateWhenActive(GetFrameTime());
    } else if (getState() == SpriteState::HIT) {
        updateWhenHit(GetFrameTime());
    }
    updateCollisionBoxes();
}

void FireFlower::updateWhenActive(float timeElapsed)
{
    frameAcum += timeElapsed;
    if (frameAcum >= frameTime) {
        frameAcum -= frameTime;
        curFrame++;
        if (curFrame >= maxFrame) {
            curFrame = 0;
        }
    }

    if (isInInventory) {
        // Falling from the inventory
        //Position
        setY(getY() + getVelocityY() * timeElapsed);

        //Blinking effect
        blinkFrameAccum += timeElapsed;
        if (blinkFrameAccum >= blinkFrameTime) {
            blinkFrameAccum -= blinkFrameTime;
            appear = !appear; // Toggle appearance
        }
    } 
}

void FireFlower::updateWhenHit(float timeElapsed)
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
    if (pointFrameAccum >= pointFrameTime)
    {
        pointFrameAccum = pointFrameTime;
    }
}

void FireFlower::draw()
{
    if (this->getState() == SpriteState::ACTIVE)
    {
        if (isInInventory && appear) {
            DrawTexture(ResourceManager::getTexture()["FireFlower" + std::to_string(this->getCurrentFrame())], this->getX(), this->getY(), this->getColor());
        } else if (!isInInventory) {
            DrawTexture(ResourceManager::getTexture()["FireFlower" + std::to_string(this->getCurrentFrame())], this->getX(), this->getY(), this->getColor());
        }
    }
    else if (this->getState() == SpriteState::HIT)
    {
        //Draw point floating above the flower
    }
}

void FireFlower::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["PowerUp"]);
}

void FireFlower::updateCharacter(Character *character)
{
    /*Add point to the character*/

    CharacterType type = character->getType();
    if (type == CharacterType::SMALL) {
        character->setY(character->getY() - 16); 
        character->setPreviousState(character->getState());
        character->setState(SpriteState::SMALL_TO_FLOWER);
    } else if (type == CharacterType::SUPER) {
        character->setPreviousState(character->getState());
        character->setState(SpriteState::SUPER_TO_FLOWER);
        // ??? if inventory is empty, put mushroom into the inventory ???
        // CharacterType previousType = character->getPreviousType();
        // if (previousType == CharacterType::SMALL) {
        //     PlaySound(ResourceManager::getSound()["StorePowerUpItem"]);
        //     /*Put Mushroom into the inventory*/
        // }
    } else if (type == CharacterType::FLOWER) {
        // ??? if inventory is empty, put flower into the inventory ???
        // CharacterType previousType = character->getPreviousType();
        // if (previousType == CharacterType::SMALL || previousType == CharacterType::SUPER) {
        //     PlaySound(ResourceManager::getSound()["StorePowerUpItem"]);
        //     /*Put Fireflower into the inventory*/
        // }
    }
}

void FireFlower::collisionSouth(Character *character)
{
    if (isInInventory) {
        isInInventory = false;
    }
}
