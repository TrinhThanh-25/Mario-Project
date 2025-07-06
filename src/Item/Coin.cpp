#include "Item/Coin.h"
#include "Coin.h"

Coin::Coin(Vector2 position, Vector2 size, Vector2 vel, Color color, float FrameTime, int MaxFrame, Direction direction, float HitFrameTime, int maxHitFrame, bool pause, int points):
    Item(position, size, vel, color, FrameTime, MaxFrame, direction, HitFrameTime, maxHitFrame, pause), points(points)  
{
}

void Coin::update()
{
    if (this->getState() == SpriteState::ACTIVE)
    {
        float timeElapsed = GetFrameTime();
        this->frameAcum += timeElapsed;
        if (this->frameAcum >= this->frameTime) {
            this->frameAcum -= this->frameTime;
            this->curFrame++;
            if (this->curFrame >= this->maxFrame) {
                this->curFrame = 0;
            }
        }
    } else if (this->getState() == SpriteState::HIT)
    {
        this->beingHitFrameAcum += GetFrameTime();
        if (this->beingHitFrameAcum >= this->beingHitFrameTime) {
            this->beingHitFrameAcum -= this->beingHitFrameTime;
            this->currentBeingHitFrame++;
            if (this->currentBeingHitFrame >= this->maxBeingHitFrame) {
                this->setState(SpriteState::TO_BE_REMOVED);
            }
        }
    }
}

void Coin::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["Coin" + std::to_string(this->getCurrentFrame())], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->getCurrentBeingHitFrame())], this->getX(), this->getY(), this->getColor());
    }
}

void Coin::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["Coin"]);
}

void Coin::updateMario(Mario &mario)
{
    // coin + 1
    // point + 
}
