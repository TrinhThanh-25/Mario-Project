#include "Item/CourseClearToken.h"

CourseClearToken::CourseClearToken(Vector2 position, Vector2 size, Color color):
    Item(position, size, {0, 0}, color, 0.0f, 0, Direction::RIGHT, 0.1f, 4, false),
    minY(position.y),
    maxY(minY + 8 * size.y),
    points(8000)
{
}

void CourseClearToken::update()
{
    float timeElapsed = GetFrameTime(); 
    if (state == SpriteState::ACTIVE) {
        updateWhenActive(timeElapsed);
    } else if (state == SpriteState::HIT) {
        updateWhenHit(timeElapsed);
    }
    updateCollisionBoxes();
}

void CourseClearToken::updateWhenActive(float timeElapsed)
{
    if ( getY() < minY ) {
        setY(minY);
        this->setVelocityY(-this->getVelocityY());
    } else if ( getY() > maxY ) {
        setY(maxY);
        this->setVelocityY(-this->getVelocityY());
    }
    setY(getY() + this->getVelocityY() * timeElapsed);
}

void CourseClearToken::updateWhenHit(float timeElapsed)
{
    beingHitFrameAcum += timeElapsed;
    if ( beingHitFrameAcum >= beingHitFrameTime ) {
        beingHitFrameAcum = 0;
        currentBeingHitFrame++;
        if ( currentBeingHitFrame == maxBeingHitFrame ) {
            state = SpriteState::TO_BE_REMOVED;
        }
    }
    pointFrameAccum += timeElapsed;
    if ( pointFrameAccum >= pointFrameTime ) {
        pointFrameAccum = pointFrameTime;
    }
}

void CourseClearToken::draw()
{
    if (this->getState() == SpriteState::ACTIVE)
    {

        DrawTexture(ResourceManager::getTexture()["CourseClearToken"], 
                    this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        DrawTexture(ResourceManager::getTexture()["Gui8000"], 
                    this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui8000"].width / 2,
                    this->getY() - ResourceManager::getTexture()["Gui8000"].height - (50 * pointFrameAccum / pointFrameTime),
                    WHITE);
    }
}

void CourseClearToken::playCollisionSound()
{
}

void CourseClearToken::updateCharacter(Character *character)
{
    GameHud::addPoints(points);
    character->setState(SpriteState::VICTORY);
}

void CourseClearToken::collisionSouth(Character *character)
{
}
