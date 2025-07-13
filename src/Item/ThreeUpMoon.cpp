#include "Item/ThreeUpMoon.h"

ThreeUpMoon::ThreeUpMoon(Vector2 position, Vector2 size, Color color, int lives):
Item(position, size, Vector2(0, 0), color, 0.1f, 2, Direction::RIGHT, 0.1f, 4, false), lives(lives)
{
    pauseGameWhenHit = false;
}

void ThreeUpMoon::update()
{
    float timeElapsed = GetFrameTime();
    if (state == SpriteState::ACTIVE) {
        updateWhenActive(timeElapsed);
    } else if (state == SpriteState::BEING_HIT) {
        updateWhenHit(timeElapsed);
    }
    updateCollisionBoxes();
}

void ThreeUpMoon::updateWhenActive(float timeElapsed)
{
    frameAcum += timeElapsed;
    if (frameAcum >= frameTime) {
        frameAcum -= frameTime;
        curFrame++;
        if (curFrame >= maxFrame) {
            curFrame = 0;
        }
    }
}

void ThreeUpMoon::updateWhenHit(float timeElapsed)
{
    beingHitFrameAcum += timeElapsed;
    if (beingHitFrameAcum >= beingHitFrameTime) {
        beingHitFrameAcum -= beingHitFrameTime;
        currentBeingHitFrame++;
        if (currentBeingHitFrame >= maxBeingHitFrame) {
            currentBeingHitFrame = 0;
        }
    }
    pointFrameAccum += timeElapsed;
    if (pointFrameAccum >= pointFrameTime) { pointFrameAccum = pointFrameTime; }
}

void ThreeUpMoon::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["3UpMoon"], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        //Draw 3up 
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->getCurrentBeingHitFrame())], this->getX(), this->getY(), this->getColor());
    }
}

void ThreeUpMoon::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["1up"]);
}

void ThreeUpMoon::updateCharacter(Character *character)
{
    //Add 3 lives to character
}
