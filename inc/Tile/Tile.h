#ifndef TILE_H
#define TILE_H

#include "Common/Sprite.h"
#include "raylib.h"
#include <string>

enum TileType {
    SOLID,
    SOLID_ONLY_ENEMY,
    NOT_SOLID,
    SOLID_ABOVE,
    SLOPE_UP,
    SLOPE_DOWN,
};

class Tile : public Sprite {
    private:
        std::string nameTexture;
        TileType type;
    public:
        Tile(Vector2 position, Vector2 size, std::string nameTexture, TileType type);
        ~Tile() override;

        void update() override;
        void draw() override;

        TileType getType() const;
};

#endif