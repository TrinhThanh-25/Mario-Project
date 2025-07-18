#include "Item/ThreeUpMoon.h"

ThreeUpMoon::ThreeUpMoon(Vector2 position, Vector2 size, Color color, int lives):
Item(position, size, {0, 0}, color, 0, 0, Direction::RIGHT, 0, 0, false), lives(lives)
{
    pauseGameWhenHit = false;
    type = ItemType::THREE_UP_MOON;
}

void ThreeUpMoon::update()
{
    float timeElapsed = GetFrameTime();
    if (state == SpriteState::ACTIVE) {
        updateWhenActive(timeElapsed);
    } else if (state == SpriteState::HIT) {
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
        DrawTexture(
            ResourceManager::getTexture()["Gui3Up"],
            this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui3Up"].width / 2,
            this->getY() - ResourceManager::getTexture()["Gui3Up"].height - (50 * pointFrameAccum / pointFrameTime),
            WHITE
        );
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->currentBeingHitFrame)], this->getX(), this->getY(), this->getColor());
    }
}

void ThreeUpMoon::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["1up"]);
}

void ThreeUpMoon::updateCharacter(Character *character)
{
    character->getGameHud()->addLives(this->lives);
}

void ThreeUpMoon::collisionSouth(Character *character)
{
}
