#ifndef TILE_H
#define TILE_H

#include "Tile/TileType.h"
#include "Common/Sprite.h"
#include "raylib.h"
#include <string>

class Tile : public Sprite {
    private:
        std::string nameTexture;
        TileType type;
    public:
        Tile(Vector2 position, Vector2 size, std::string nameTexture);
        Tile(Vector2 position, Vector2 size, std::string nameTexture, TileType type);
        ~Tile() override;

        void update() override;
        void draw() override;

        TileType getType() const;
};

#endif