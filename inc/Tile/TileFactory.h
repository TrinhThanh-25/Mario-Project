#ifndef TILEFACTORY_H
#define TILEFACTORY_H

#include "Tile/Tile.h"

class TileFactory {
    public:
        static Tile* createTile(Vector2 position, std::string nameTexture, TileType type = TileType::SOLID);
};

#endif