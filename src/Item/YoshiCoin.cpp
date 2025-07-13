#include "Item/YoshiCoin.h"

YoshiCoin::YoshiCoin(Vector2 position, Vector2 size, Color color, int points):
Item(position, size, Vector2(0, 0), color, 0.1f, 4, Direction::RIGHT, 0.1f, 4, false), points(points)
{
}

void YoshiCoin::update()
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

void YoshiCoin::updateWhenActive(float timeElapsed)
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

void YoshiCoin::updateWhenHit(float timeElapsed)
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

void YoshiCoin::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["YoshiCoin" + std::to_string(this->getCurrentFrame())], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        //Draw point floating above the coin
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->getCurrentBeingHitFrame())], this->getX(), this->getY(), this->getColor());
    }
}

void YoshiCoin::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["DragonCoin"]);
}

void YoshiCoin::updateCharacter(Character *character)
{
    /*Add point here*/
    /*If there is 5 coins --> add one live*/
}
