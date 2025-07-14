#include "Item/OneUpMushroom.h"
#include "Game/World.h"

OneUpMushroom::OneUpMushroom(Vector2 position, Vector2 size, Color color, int lives):
Item(position, size, {320, 0}, color, 0.1f, 2, Direction::RIGHT, 0, 0, false), lives(lives)
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
        DrawTexture(
            ResourceManager::getTexture()["Gui1Up"],
            this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui1Up"].width / 2,
            this->getY() - ResourceManager::getTexture()["Gui1Up"].height - (50 * pointFrameAccum / pointFrameTime),
            WHITE
        );
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->currentBeingHitFrame)], this->getX(), this->getY(), this->getColor());
    }
}

void OneUpMushroom::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["OneUpMushroomHit"]);
}

void OneUpMushroom::updateCharacter(Character *character)
{
    character->getGameHud()->addLives(this->lives);
}

void OneUpMushroom::collisionSouth(Character *character)
{
}
