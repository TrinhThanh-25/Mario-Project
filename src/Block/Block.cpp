#include "Block/Block.h"

Rectangle Block::getCollisionBox() {
    return Rectangle{ position.x, position.y, size.x, size.y };
}
