#include "Item/OneUpMushroom.h"

OneUpMushroom::OneUpMushroom(Vector2 position, Vector2 size, Color color, int lives):
Item(position, size, Vector2(320, 0), color, 0.1f, 2, Direction::RIGHT, 0, 0, false), lives(lives)
{
    pauseGameWhenHit = false;
}

void OneUpMushroom::update()
{
    if (getState() == SpriteState::ACTIVE)
    {
        updateWhenActive(GetFrameTime());
    }
    else if (getState() == SpriteState::HIT)
    {
        updateWhenHit(GetFrameTime());
    }
    updateCollisionBoxes();
}

void OneUpMushroom::updateWhenActive(float timeElapsed)
{
    setVelocityY(getVelocityY() + World::gravity * timeElapsed);
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

void OneUpMushroom::updateWhenHit(float timeElapsed)
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

void OneUpMushroom::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["OneUpMushroom"], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        // Draw 1up mushroom hit effect
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->getCurrentBeingHitFrame())], this->getX(), this->getY(), this->getColor());
    }
}

void OneUpMushroom::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["OneUpMushroomHit"]);
}

void OneUpMushroom::updateCharacter(Character *character)
{
    //Add lives to the character
}
