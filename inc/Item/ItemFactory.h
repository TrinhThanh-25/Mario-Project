#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "Item.h"
#include "Coin.h"
#include "Mushroom.h"
#include "FireFlower.h"
#include "Star.h"
#include "YoshiCoin.h"
#include "ThreeUpMoon.h"
#include "OneUpMushroom.h"
#include "CourseClearToken.h"


enum class Source {
    BLOCK,
    INVENTORY,
};

class ItemFactory {
public:
    static Item* createItem(ItemType type, Source source, Vector2 position, Direction direction);
};

#endif // ITEMFACTORY_H