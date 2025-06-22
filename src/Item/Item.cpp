#include "Item/Item.h"
#include "Item.h"

Item::Item() {}

Item::~Item()
{
    Sprite::~Sprite();
}

bool Item::isPausedGameWhenBeingHit()
{
    return pauseGameWhenHit;
}
