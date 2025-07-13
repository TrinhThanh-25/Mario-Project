#include "Item/Star.h"
#include "Game/World.h"

Star::Star(Vector2 position, Vector2 size, Color color, int points):
Item(position, size, {300, 0}, color, 0.1f, 4, Direction::RIGHT, 0.1f, 4, false), points(points)
{
}

void Star::update()
{
    if (this->getState() == SpriteState::ACTIVE)
    {
        this->updateWhenActive(GetFrameTime());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        this->updateWhenHit(GetFrameTime());
    }
    updateCollisionBoxes();
}

void Star::updateWhenActive(float timeElapsed)
{
    frameAcum += timeElapsed;
    if (getDirection() == Direction::RIGHT)
    {
        this->setX(this->getX() + this->getVelocityX() * timeElapsed);
    }
    else if (getDirection() == Direction::LEFT)
    {
        this->setX(this->getX() - this->getVelocityX() * timeElapsed);
    }
    this->setY(this->getY() + this->getVelocityY() * timeElapsed);
    this->setVelocityY(this->getVelocityY() + gravity * timeElapsed);
}

void Star::updateWhenHit(float timeElapsed)
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

void Star::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->curFrame)], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        DrawTexture(
            ResourceManager::getTexture()["Gui400"],
            this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui400"].width / 2,
            this->getY() - ResourceManager::getTexture()["Gui400"].height - (50 * pointFrameAccum / pointFrameTime),
            WHITE
        );
    }
}

void Star::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["Star"]);
}

void Star::updateCharacter(Character *character)
{
    GameHud::addPoints(points);
    character->setInvincible(true);
}

void Star::collisionSouth(Character *character)
{
    setVelocityY(-400);
}
