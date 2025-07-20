#include "Item/FireFlower.h"

FireFlower::FireFlower(Vector2 position, Vector2 size, Color color, int points, bool isInInventory):
Item(position, size, {0, 0}, color, 0.1f, 2, Direction::RIGHT, 0, 0, false), points(points), isInInventory(isInInventory)
{
    pauseGameWhenHit = true;
    if (isInInventory) {
        setVelocityY(100);
    }
    type = ItemType::FLOWER;
}
void FireFlower::update()
{
    if (getState() == SpriteState::ACTIVE) {
        updateWhenActive(GetFrameTime());
    } else if (getState() == SpriteState::HIT) {
        updateWhenHit(GetFrameTime());
    }
    updateCollisionBoxes();
}

void FireFlower::updateWhenActive(float timeElapsed)
{
    frameAcum += timeElapsed;
    if (frameAcum >= frameTime) {
        frameAcum -= frameTime;
        curFrame++;
        if (curFrame >= maxFrame) {
            curFrame = 0;
        }
    }

    if (isInInventory) {
        setY(getY() + getVelocityY() * timeElapsed);
        blinkFrameAccum += timeElapsed;
        if (blinkFrameAccum >= blinkFrameTime) {
            blinkFrameAccum -= blinkFrameTime;
            appear = !appear; // Toggle appearance
        }
    } 
}

void FireFlower::updateWhenHit(float timeElapsed)
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

void FireFlower::draw()
{
    if (this->getState() == SpriteState::ACTIVE)
    {
        if (isInInventory && appear) {
            DrawTexture(ResourceManager::getTexture()["FireFlower" + std::to_string(this->curFrame)], this->getX(), this->getY(), this->getColor());
        } else if (!isInInventory) {
            DrawTexture(ResourceManager::getTexture()["FireFlower" + std::to_string(this->curFrame)], this->getX(), this->getY(), this->getColor());
        }
    }
    else if (this->getState() == SpriteState::HIT)
    {
        // Draw point floating above the flower
        DrawTexture(ResourceManager::getTexture()["Gui300"],
                    this->getX() + this->getWidth() / 2 - ResourceManager::getTexture()["Gui300"].width / 2,
                    this->getY() - ResourceManager::getTexture()["Gui300"].height - (50 * pointFrameAccum / pointFrameTime),
                    WHITE);
    }
}

void FireFlower::playCollisionSound()
{
    PlaySound(ResourceManager::getSound()["PowerUp"]);
}

void FireFlower::updateCharacter(Character *character)
{
    character->getGameHud()->addPoints(points);
    CharacterType type = character->getType();
    if (type == CharacterType::SMALL) {
        character->setY(character->getY() - 16); 
        character->setPreviousState(character->getState());
        character->setState(SpriteState::SMALL_TO_FLOWER);
    } else if (type == CharacterType::SUPER) {
        character->setPreviousState(character->getState());
        character->setState(SpriteState::SUPER_TO_FLOWER);
        // ??? if inventory is empty, put mushroom into the inventory ???
        CharacterType previousType = character->getGameHud()->getPowerUpItem();
        if (previousType == CharacterType::SMALL) {
            PlaySound(ResourceManager::getSound()["StorePowerUpItem"]);
            character->getGameHud()->setPowerUpItem(CharacterType::SUPER);
        }
    } else if (type == CharacterType::FLOWER) {
        CharacterType previousType = character->getGameHud()->getPowerUpItem();
        if (previousType == CharacterType::SMALL || previousType == CharacterType::SUPER) {
            PlaySound(ResourceManager::getSound()["StorePowerUpItem"]);
            character->getGameHud()->setPowerUpItem(CharacterType::FLOWER);
        }
    }
}

void FireFlower::collisionSouth(Character *character)
{
    if (isInInventory) {
        isInInventory = false;
    }
}

json FireFlower::saveToJson() const
{
    json j = Item::saveToJson();
    j["points"] = points;
    j["isInInventory"] = isInInventory;
    j["appear"] = appear;
    return j;
}

void FireFlower::loadFromJson(const json &j)
{
    Item::loadFromJson(j);
    points = j.value("points", 0);
    isInInventory = j.value("isInInventory", false);
    appear = j.value("appear", false);
}
