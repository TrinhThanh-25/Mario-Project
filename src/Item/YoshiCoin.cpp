#include "Item/YoshiCoin.h"

YoshiCoin::YoshiCoin(Vector2 position, Vector2 size, Color color, int points):
Item(position, size, {0, 0}, color, 0.1f, 4, Direction::RIGHT, 0.1f, 4, false), points(points)
{
    type = ItemType::YOSHI_COIN;
}

void YoshiCoin::update()
{
    float timeElapsed = GetFrameTime();
    if (this->getState() == SpriteState::IDLE || this->getState() == SpriteState::ACTIVE)
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
        DrawTexture(ResourceManager::getTexture()["YoshiCoin" + std::to_string(this->curFrame)], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        DrawTexture(
            ResourceManager::getTexture()["Gui400"],
            this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui400"].width / 2,
            this->getY() - ResourceManager::getTexture()["Gui400"].height - (50 * pointFrameAccum / pointFrameTime),
            WHITE
        );
        DrawTexture(ResourceManager::getTexture()["Stardust" + std::to_string(this->currentBeingHitFrame)], this->getX(), this->getY(), this->getColor());
    }
}

void YoshiCoin::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["DragonCoin"]);
}

void YoshiCoin::updateCharacter(Character *character)
{
    /*Add point here*/
    character->getGameHud()->addPoints(points);
    /*If there is 5 coins --> add one live*/
    character->getGameHud()->addYoshiCoins(1);
    if (character->getGameHud()->getYoshiCoins() >= 5)
    {
        character->getGameHud()->addLives(1);
        character->getGameHud()->setYoshiCoins(0); // Reset Yoshi coins after gaining a life
    }
}

void YoshiCoin::collisionSouth(Character *character)
{
}
void YoshiCoin::loadFromJson(const json &j)
{
    Item::loadFromJson(j);
    points = j.value("points", 0);
}
json YoshiCoin::saveToJson() const
{
    json j = Item::saveToJson();
    j["points"] = points;
    return j;
}
