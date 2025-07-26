#include "Tile/Tile.h"
#include "Common/ResourceManager.h"

Tile::Tile(Vector2 position, Vector2 size, std::string nameTexture, TileType type)
    : Sprite(position, size, BLUE), 
    nameTexture(nameTexture), 
    type(type) {
};

Tile::~Tile() = default;

void Tile::update() {}

void Tile::draw() {
    DrawTexture(ResourceManager::getTexture()[nameTexture], position.x, position.y, WHITE);
}

TileType Tile::getType() const {
    return type;
}

json Tile::saveToJson() const {
    json j = Sprite::saveToJson();
    j["nameTexture"] = nameTexture;
    j["type"] = static_cast<int>(type);
    return j;
}

void Tile::loadFromJson(const json& j) {
    Sprite::loadFromJson(j);
    nameTexture = j["nameTexture"].get<std::string>();
    type = static_cast<TileType>(j["type"].get<int>());
}