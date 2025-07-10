#include "Item/CourseClearToken.h"

CourseClearToken::CourseClearToken(Vector2 position, Vector2 size, Color color):
    Item(position, size, Vector2(0, 0), color, 0.0f, 0, Direction::NONE, 0.1f, 4, false),
    minY(position.y),
    maxY(minY + 8 * size.y),
    points(10000)
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
        this->setVelocityY(-this->getVelocityY().y);
    } else if ( getY() > maxY ) {
        setY(maxY);
        this->setVelocityY(-this->getVelocityY().y);
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
        // Draw the course clear token
        DrawTexture(ResourceManager::getTexture()["CourseClearToken"], 
                    this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        //Draw point floating above the flower
    }
}

void CourseClearToken::playCollisionSound()
{
    //PlaySound(ResourceManager::getSound()["CourseClearToken"]);
}

void CourseClearToken::updateCharacter(Character *character)
{
    //Add points to the character
    character->setState(SpriteState::VICTORY);
}
