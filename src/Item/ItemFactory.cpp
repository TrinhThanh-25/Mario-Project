#include "Item/ItemFactory.h"

Item* ItemFactory::createItem(ItemType type, Source source, Vector2 position, Direction direction)
{
    Item* item = nullptr;
    switch (type) {
        case ItemType::COIN:
            item = new Coin(position, Vector2(32, 32), WHITE, 100); // Assuming default points for coin
            break;
        case ItemType::MUSHROOM:
            if (source == Source::BLOCK) {
                item = new Mushroom(position, Vector2(32, 32), WHITE, 200, false); // Assuming default points for mushroom
            } else {
                item = new Mushroom(position, Vector2(32, 32), WHITE, 200, true); // For inventory
            }
            item->setDirection(direction);
            break;
        case ItemType::FLOWER:
            if (source == Source::BLOCK) {
                item = new FireFlower(position, Vector2(32, 32), WHITE, 300, false); // Assuming default points for flower
            } else {
                item = new FireFlower(position, Vector2(32, 32), WHITE, 300, true); // For inventory
            }
            break;
        case ItemType::STAR:
            item = new Star(position, Vector2(32, 32), WHITE, 400); // Assuming default points for star
            item->setDirection(direction);
            break;
        case ItemType::YOSHI_COIN:
            item = new YoshiCoin(position, Vector2(32, 32), WHITE, 500); // Assuming default points for Yoshi coin
            break;
        case ItemType::THREE_UP_MOON:
            item = new ThreeUpMoon(position, Vector2(32, 32), WHITE, 3); // Assuming 3 lives for Three Up Moon
            break;
        case ItemType::ONE_UP_MUSHROOM:
            item = new OneUpMushroom(position, Vector2(32, 32), WHITE, 1); // Assuming 1 life for One Up Mushroom
            break;
        case ItemType::COURSE_CLEAR_TOKEN:
            item = new CourseClearToken(position, Vector2(32, 32), WHITE); // Assuming default points for Course Clear Token
            break;
    }
    return item;
}