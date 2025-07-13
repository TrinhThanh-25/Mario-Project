#include "Item/Coin.h"

Coin::Coin(Vector2 position, Vector2 size, Color color, int points):
Item(position, size, {0, 0}, color, 0.1f, 4, Direction::RIGHT, 0.1f, 4, false), points(points) {}

void Coin::update()
{
    float timeElapsed = GetFrameTime();
    if (this->getState() == SpriteState::IDLE)
    {
        this->updateWhenActive(timeElapsed);
    }
    else if (this->getState() == SpriteState::HIT)
    {
        this->updateWhenHit(timeElapsed);
    }
    updateCollisionBoxes();
}

void Coin::updateWhenActive(float timeElapsed)
{
    frameAcum += timeElapsed;
    if (frameAcum >= frameTime)
    {
        frameAcum -= frameTime;
        curFrame++;
        if (curFrame >= maxFrame)
        {
            curFrame = 0;
        }
    }
}

void Coin::updateWhenHit(float timeElapsed)
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

void Coin::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["Coin" + std::to_string(this->getCurrentFrame())], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        //Draw point floating above the coin
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->getCurrentBeingHitFrame())], this->getX(), this->getY(), this->getColor());
    }
}

void Coin::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["Coin"]);
}

void Coin::updateCharacter(Character *character)
{
    //Add points to the character
    //Add coin to the character's inventory
}


