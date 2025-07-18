#include "Tile/TileFactory.h"

Tile* TileFactory::createTile(Vector2 position, std::string nameTexture, TileType type) {
    return new Tile(position, {32.0f, 32.0f}, nameTexture, type);
}