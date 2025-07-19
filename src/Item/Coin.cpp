#include "Item/Coin.h"
#include "Game/World.h"

Coin::Coin(Vector2 position, Vector2 size, Color color, int points):
Item(position, size, {0,0}, color, 0.1f, 4, Direction::RIGHT, 0.1f, 4, false), points(points) {
    type = ItemType::COIN;
}

void Coin::update()
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

void Coin::updateWhenActive(float timeElapsed)
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

void Coin::updateWhenHit(float timeElapsed)
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

void Coin::draw()
{
    if (this->getState() == SpriteState::ACTIVE || this->getState() == SpriteState::IDLE)
    {
        DrawTexture(ResourceManager::getTexture()["Coin" + std::to_string(this->curFrame)], this->getX(), this->getY(), this->getColor());
    }
    else if (this->getState() == SpriteState::HIT)
    {
        DrawTexture(
            ResourceManager::getTexture()["Gui100"],
            this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui100"].width / 2,
            this->getY() - ResourceManager::getTexture()["Gui100"].height - (50 * pointFrameAccum / pointFrameTime),
            WHITE
        );
        DrawTexture(ResourceManager::getTexture()["Star" + std::to_string(this->currentBeingHitFrame)], this->getX(), this->getY(), this->getColor());
    }
}

void Coin::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["Coin"]);
}

void Coin::updateCharacter(Character *character)
{
    character->getGameHud()->addCoins(1);
    character->getGameHud()->addPoints(points);
}

void Coin::collisionSouth(Character *character)
{
}

json Coin::saveToJson() const
{
    json j = Item::saveToJson();
    j["points"] = points;
    return j;
}

void Coin::loadFromJson(const json& j)
{
    Item::loadFromJson(j);
    points = j.value("points", 0);
}
