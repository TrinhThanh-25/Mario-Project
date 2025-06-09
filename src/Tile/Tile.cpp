#include "Tile/Tile.h"
#include "Common/ResourceManager.h"

Tile::Tile(Vector2 position, Vector2 size, std::string nameTexture)
    : Tile(position, size, nameTexture,TileType::SOLID) {
};

Tile::Tile(Vector2 position, Vector2 size, std::string nameTexture, TileType type)
    : Sprite(position, size, WHITE), nameTexture(nameTexture), type(type) {
};

Tile::~Tile() = default;

void Tile::update() {}

void Tile::draw() {
    DrawTexture(ResourceManager::getTexture()[nameTexture], position.x, position.y, WHITE);
}

TileType Tile::getType() const {
    return type;
}