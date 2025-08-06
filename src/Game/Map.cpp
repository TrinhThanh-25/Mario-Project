#include "Tile/TileFactory.h"
#include "Block/BlockFactory.h"
#include "Item/ItemFactory.h"
#include "Enemy/EnemyFactory.h"
#include "Game/Map.h"
#include "Character/Character.h"
#include "Common/ResourceManager.h"
#include "raylib.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

Map::Map(std::vector<Character*>& characters, World* world, int mapNumber)
    : characters(characters), 
    world(world), 
    mapNumber(mapNumber),
    maxMapNumber(4),
    offset(0.0f), 
    camera(nullptr), 
    width(0), 
    height(0),
    background(Texture2D()),
    backgroundColor(WHITE),
    backgroundID(0),
    musicID(0){
}

Map::~Map() {
    clear();
}

void Map::loadMap(int mapNumber) {
    background = ResourceManager::getTexture()["Background" + std::to_string(mapNumber)];
    backgroundColor = WHITE;
    backgroundID = mapNumber;
    musicID = mapNumber;
    loadMap("Map" + std::to_string(mapNumber));
    this->mapNumber = mapNumber;
}

void Map::loadMap(const std::string& mapFileName) {
    this->mapFileName = mapFileName;
    std::string fileName = "../resources/Map/" + mapFileName + ".json";
    std::ifstream file(fileName);
	if (!file) {
		std::cerr << "Could not open json file " << fileName << std::endl;
		return;
	}
	nlohmann::json mapJson;
	file >> mapJson;
	int width = mapJson["width"];
	int height = mapJson["height"];
	this->width = (float) width * 32.0f;
    this->height = (float) height * 32.0f;
	float tilewidth = 32.0f;
	std::vector<int> data = mapJson["layers"][0]["data"];
	loadObjectsToMap(data);
}

void Map::setCharacters(std::vector<Character*>& characters) {
    this->characters = characters;
}

void Map::draw() {
    if(world->getGamePlay() == GamePlay::PLAYDEVELOPEDMAP) {
        background = ResourceManager::getTexture()["Background" + std::to_string(backgroundID)];
        DrawRectangle(0, 0, width, height, backgroundColor);
        int repeat = width / background.width + 2;
        if ( backgroundID > 0 ) {
        for ( int i = 0; i <= repeat; i++ ) {
            DrawTexture(background,-background.width + i * background.width + offset * 0.5, height - background.height, WHITE );
        }
    }
    }
    for (auto& bT : backTile) {
        bT->draw();
    }
    for (auto& bE : backEnemy) {
        bE->draw();
    }
    for (auto& t : tile) {
        t->draw();
    }
    for (auto& b : block) {
        b->draw();
    }
    for (auto& mB : messBlock) {
        mB->draw();
    }
    for (auto& i : item) {
        i->draw();
    }
    for (auto& sI : staticItem) {
        sI->draw();
    }
    for (auto& fE : frontEnemy) {
        fE->draw();
    }
    for (auto& character : characters) {
        character->draw();
    }
    for (auto& fT : frontTile) {
        fT->draw();
    }
    if (netMode) {
        int columns = width / 32;
        int lines = height / 32;
        for ( int i = 0; i < lines; i++ ) {
            DrawLine( 0, i * 32, width, i * 32, BLACK );
        }
        for ( int i = 0; i < columns; i++ ) {
            DrawLine( i * 32, 0, i * 32, height, BLACK );
        }
    }
}

void Map::setOffset(float offset) {
    this->offset = offset;
}

void Map::setCamera(Camera2D* camera) {
    this->camera = camera;
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

std::vector<Tile*>& Map::getTile() {
    return tile;
}

std::vector<Block*>& Map::getBlock() {
    return block;
}

std::vector<Enemy*>& Map::getBackEnemy() {
    return backEnemy;
}

std::vector<Enemy*>& Map::getFrontEnemy() {
    return frontEnemy;
}

std::vector<Item*>& Map::getItem() {
    return item;
}

std::vector<Item*>& Map::getStaticItem() {
    return staticItem;
}

void Map::clear() {
    for (auto& t : tile) {
        if(t)
        delete t;
        t = nullptr;
    }
    tile.clear();
    for (auto& bT : backTile) {
        if(bT)
        delete bT;
        bT = nullptr;
    }
    backTile.clear();
    for (auto& fT : frontTile) {
        if(fT)
        delete fT;
        fT = nullptr;
    }
    frontTile.clear();
    for (auto& b : block) {
        if(b)
        delete b;
        b = nullptr;
    }
    block.clear();
    for (auto& mB : messBlock) {
        if(mB)
        delete mB;
        mB = nullptr;
    }
    messBlock.clear();
    for (auto& bE : backEnemy) {
        if(bE)
        delete bE;
        bE = nullptr;
    }
    backEnemy.clear();
    for (auto& fE : frontEnemy) {
        if(fE)
        delete fE;
        fE = nullptr;
    }
    frontEnemy.clear();
    for (auto& i : item) {
        if(i)
        delete i;
        i = nullptr;
    }
    item.clear();
    for (auto& sI : staticItem) {
        if(sI)
        delete sI;
        sI = nullptr;
    }
    staticItem.clear();
}

void Map::showMessage() {
    // Hiện thông báo nếu có
}

void Map::reset(bool isTestMap) {
    if(!isTestMap) {
        StopMusicStream(ResourceManager::getMusic()["Invincible"]);
        StopMusicStream(ResourceManager::getMusic()["Music" + std::to_string(musicID)]);
        loadMap(mapFileName);
    }
    else {
        loadObjectsToMap(mapGrid);
    }
}

bool Map::next() {
    if (mapNumber < maxMapNumber) {
        mapNumber++;
        mapFileName = "Map" + std::to_string(mapNumber);
        reset();
        return true;
    }
    return false;
}

void Map::first() {
    mapNumber = 1;
    reset();
}

void Map::playMusic() {
    if (musicID > 0) {
        std::unordered_map<std::string, Music>& music = ResourceManager::getMusic();
        bool check = false;
        for (auto& character : characters) {
            if (character->isInvincible()) {
                check = true;
                break;
            }
        }
        if(check) {
            if (!IsMusicStreamPlaying(music["Invincible"])) {
                StopMusicStream(music["Music" + std::to_string(musicID)]);
                PlayMusicStream(music["Invincible"]);
            }
            else {
                UpdateMusicStream(music["Invincible"]);
            }
        } else {
            if (!IsMusicStreamPlaying(music["Music" + std::to_string(musicID)])) {
                StopMusicStream(music["Invincible"]);
                PlayMusicStream(music["Music" + std::to_string(musicID)]);
            }
            else {
                UpdateMusicStream(music["Music" + std::to_string(musicID)]);
            }
        }
    }
}

void Map::stopMusic() {
    StopMusicStream(ResourceManager::getMusic()["Invincible"]);
    StopMusicStream(ResourceManager::getMusic()["Music" + std::to_string(musicID)]);
}

json Map::saveToJson() const {
    json j;
    j["mapFileName"] = mapFileName;
    j["offset"] = offset;
    j["mapNumber"] = mapNumber;
    j["maxMapNumber"] = maxMapNumber;
    j["width"] = width;
    j["height"] = height;
    j["backgroundID"] = backgroundID;
    j["backgroundColor"] = {
        {"r", backgroundColor.r},
        {"g", backgroundColor.g},
        {"b", backgroundColor.b},
        {"a", backgroundColor.a}
    };
    j["musicID"] = musicID;
    j["tiles"] = json::array();
    for (const auto& t : tile) {
        j["tiles"].push_back(t->saveToJson());
    }
    j["backTiles"] = json::array();
    for (const auto& bT : backTile) {
        j["backTiles"].push_back(bT->saveToJson());
    }
    j["frontTiles"] = json::array();
    for (const auto& fT : frontTile) {
        j["frontTiles"].push_back(fT->saveToJson());
    }
    j["blocks"] = json::array();
    for (const auto& b : block) {
        j["blocks"].push_back(b->saveToJson());
    }
    j["messBlocks"] = json::array();
    for (const auto& mB : messBlock) {
        j["messBlocks"].push_back(mB->saveToJson());
    }
    j["backEnemies"] = json::array();
    for (const auto& bE : backEnemy) {
        j["backEnemies"].push_back(bE->saveToJson());
    }
    j["frontEnemies"] = json::array();
    for (const auto& fE : frontEnemy) {
        j["frontEnemies"].push_back(fE->saveToJson());
    }
    j["items"] = json::array();
    for (const auto& i : item) {
        j["items"].push_back(i->saveToJson());
    }
    j["staticItems"] = json::array();
    for (const auto& sI : staticItem) {
        j["staticItems"].push_back(sI->saveToJson());
    }
    return j;
}

void Map::loadFromJson(const json& j) {
    mapFileName = j.at("mapFileName").get<std::string>();
    offset = j.at("offset").get<float>();
    mapNumber = j.at("mapNumber").get<int>();
    maxMapNumber = j.at("maxMapNumber").get<int>();
    width = j.at("width").get<int>();
    height = j.at("height").get<int>();
    backgroundID = j.at("backgroundID").get<int>();
    backgroundColor = {
        j.at("backgroundColor").at("r").get<unsigned char>(),
        j.at("backgroundColor").at("g").get<unsigned char>(),
        j.at("backgroundColor").at("b").get<unsigned char>(),
        j.at("backgroundColor").at("a").get<unsigned char>()
    };
    musicID = j.at("musicID").get<int>();
    tile.clear();
    backTile.clear();
    frontTile.clear();
    block.clear();
    messBlock.clear();
    backEnemy.clear();
    frontEnemy.clear();
    item.clear();
    staticItem.clear();
    for (const auto& tJson : j["tiles"]) {
        Tile* t = TileFactory::createTile({tJson["position"][0].get<float>(), tJson["position"][1].get<float>()},
                                             tJson["nameTexture"].get<std::string>());
        t->loadFromJson(tJson);
        tile.push_back(t);
    }
    for (const auto& bTJson : j["backTiles"]) {
        Tile* bT = TileFactory::createTile({bTJson["position"][0].get<float>(), bTJson["position"][1].get<float>()},
                                             bTJson["nameTexture"].get<std::string>());
        bT->loadFromJson(bTJson);
        backTile.push_back(bT);
    }
    for (const auto& fTJson : j["frontTiles"]) {
        Tile* fT = TileFactory::createTile({fTJson["position"][0].get<float>(), fTJson["position"][1].get<float>()},
                                             fTJson["nameTexture"].get<std::string>());
        fT->loadFromJson(fTJson);
        frontTile.push_back(fT);
    }
    for (const auto& bJson : j["blocks"]) {
        Block* b = BlockFactory::createBlock(static_cast<BlockType>(bJson["blockType"].get<int>()),
                                             {bJson["position"][0].get<float>(), bJson["position"][1].get<float>()});
        b->loadFromJson(bJson);
        block.push_back(b);
    }
    for (const auto& mBJson : j["messBlocks"]) {
        Block* mB = BlockFactory::createBlock(static_cast<BlockType>(mBJson["blockType"].get<int>()),
                                               {mBJson["position"][0].get<float>(), mBJson["position"][1].get<float>()});
        mB->loadFromJson(mBJson);
        messBlock.push_back(mB);
    }
    for (const auto& bEJson : j["backEnemies"]) {
        Enemy* bE = EnemyFactory::createEnemy(static_cast<EnemyType>(bEJson["type"].get<int>()),
                                               {bEJson["position"][0].get<float>(), bEJson["position"][1].get<float>()},
                                               static_cast<Direction>(bEJson["direction"].get<int>()));
        bE->loadFromJson(bEJson);
        backEnemy.push_back(bE);
    }
    for (const auto& fEJson : j["frontEnemies"]) {
        Enemy* fE = EnemyFactory::createEnemy(static_cast<EnemyType>(fEJson["type"].get<int>()),
                                               {fEJson["position"][0].get<float>(), fEJson["position"][1].get<float>()},
                                               static_cast<Direction>(fEJson["direction"].get<int>()));
        fE->loadFromJson(fEJson);
        frontEnemy.push_back(fE);
    }
    for (const auto& iJson : j["items"]) {
        Item* i = ItemFactory::createItem(static_cast<ItemType>(iJson["type"].get<int>()),
                                           Source::BLOCK,
                                           {iJson["position"][0].get<float>(), iJson["position"][1].get<float>()},
                                           static_cast<Direction>(iJson["direction"].get<int>()));
        i->loadFromJson(iJson);
        item.push_back(i);
    }
    for (const auto& sIJson : j["staticItems"]) {
        Item* sI = ItemFactory::createItem(static_cast<ItemType>(sIJson["type"].get<int>()),
                                             Source::BLOCK,
                                             {sIJson["position"][0].get<float>(), sIJson["position"][1].get<float>()},
                                             static_cast<Direction>(sIJson["direction"].get<int>()));
        sI->loadFromJson(sIJson);
        staticItem.push_back(sI);
    }
}

void Map::setNetMode(bool netMode) {
    this->netMode = netMode;
}

std::string Map::getMapFileName() const {
    return mapFileName;
}

void Map::setMap(int width, int height, const std::vector<int>& mapGrid) {
    this->width = width * 32;
    this->height = height * 32;
    clear();
    loadObjectsToMap(mapGrid);
}

void Map::loadObjectsToMap(const std::vector<int>& mapGrid) {
    clear();
    this->mapGrid = mapGrid;
    float tilewidth = 32.0f;
    for (int y = 0; y < height / tilewidth; ++y) {
        for (int x = 0; x < width / tilewidth; ++x) {
            int data = mapGrid[y * (width / tilewidth) + x];
            switch (data) {
            case 0:
                continue;
            case 1:
                for (auto& character : characters) {
                    if(character)
                        character->setPosition({x * tilewidth, y * tilewidth});
                }
                break;
            case 2:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1", TileType::NOT_SOLID));
                break;
            case 3:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2", TileType::SOLID_ABOVE));
                break;
            case 4:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3"));
                break;
            case 5:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4"));
                break;
            case 6:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "5"));
                break;
            case 7:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "6"));
                break;
            case 8:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "7"));
                break;
            case 9:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "8"));
                break;
            case 10:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "9", TileType::SOLID_ABOVE));
                break;
            case 11:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "10", TileType::SOLID_ABOVE));
                break;
            case 12:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "11", TileType::NOT_SOLID));
                break;
            case 13:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "12", TileType::NOT_SOLID));
                break;
            case 14:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "13"));
                break;
            case 15:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "14"));
                break;
            case 16:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "15"));
                break;
            case 17:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "16"));
                break;
            case 18:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "17"));
                break;
            case 19:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "18"));
                break;
            case 20:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "19"));
                break;
            case 21:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "20"));
                break;
            case 22:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "21"));
                break;
            case 23:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "22"));
                break;
            case 24:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "23"));
                break;
            case 25:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "24"));
                break;
            case 26:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "25"));
                break;
            case 27:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "26"));
                break;
            case 28:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "27", TileType::NOT_SOLID));
                break;
            case 29:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "28", TileType::SOLID_ABOVE));
                break;
            case 30:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "29"));
                break;
            case 31:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "30"));
                break;
            case 32:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "31"));
                break;
            case 33:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "32"));
                break;
            case 34:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "33"));
                break;
            case 35:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "34"));
                break;
            case 36:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "35", TileType::SOLID_ABOVE));
                break;
            case 37:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "36", TileType::SOLID_ABOVE));
                break;
            case 38:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "37", TileType::NOT_SOLID));
                break;
            case 39:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "38", TileType::NOT_SOLID));
                break;
            case 40:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "39"));
                break;
            case 41:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "40"));
                break;
            case 42:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "41"));
                break;
            case 43:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "42"));
                break;
            case 44:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "43"));
                break;
            case 45:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "44"));
                break;
            case 46:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "45"));
                break;
            case 47:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "46"));
                break;
            case 48:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "47"));
                break;
            case 49:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "48"));
                break;
            case 50:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "49"));
                break;
            case 51:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "50"));
                break;
            case 52:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "51"));
                break;
            case 53:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "52"));
                break;
            case 54:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "53", TileType::NOT_SOLID));
                break;
            case 55:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "54", TileType::SOLID_ABOVE));
                break;
            case 56:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "55"));
                break;
            case 57:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "56"));
                break;
            case 58:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "57"));
                break;
            case 59:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "58"));
                break;
            case 60:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "59"));
                break;
            case 61:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "60"));
                break;
            case 62:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "61", TileType::SOLID_ABOVE));
                break;
            case 63:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "62", TileType::SOLID_ABOVE));
                break;
            case 64:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "63", TileType::NOT_SOLID));
                break;
            case 65:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "64", TileType::NOT_SOLID));
                break;
            case 66:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "65"));
                break;
            case 67:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "66"));
                break;
            case 68:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "67"));
                break;
            case 69:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "68"));
                break;
            case 70:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "69"));
                break;
            case 71:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "70"));
                break;
            case 72:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "71"));
                break;
            case 73:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "72"));
                break;
            case 74:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "73"));
                break;
            case 75:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "74"));
                break;
            case 76:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "75"));
                break;
            case 77:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "76"));
                break;
            case 78:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "77"));
                break;
            case 79:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "78"));
                break;
            case 80:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "79", TileType::NOT_SOLID));
                break;
            case 81:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "80", TileType::SOLID_ABOVE));
                break;
            case 82:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "81"));
                break;
            case 83:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "82"));
                break;
            case 84:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "83"));
                break;
            case 85:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "84"));
                break;
            case 86:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "85"));
                break;
            case 87:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "86"));
                break;
            case 88:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "87", TileType::SOLID_ABOVE));
                break;
            case 89:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "88", TileType::SOLID_ABOVE));
                break;
            case 90:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "89", TileType::NOT_SOLID));
                break;
            case 91:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "90", TileType::NOT_SOLID));
                break;
            case 92:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "91"));
                break;
            case 93:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "92"));
                break;
            case 94:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "93"));
                break;
            case 95:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "94"));
                break;
            case 96:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "95"));
                break;
            case 97:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "96"));
                break;
            case 98:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "97"));
                break;
            case 99:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "98"));
                break;
            case 100:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "99"));
                break;
            case 101:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "100"));
                break;
            case 102:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "101"));
                break;
            case 103:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "102"));
                break;
            case 104:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "103"));
                break;
            case 105:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "104"));
                break;
            case 106:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "", TileType::SOLID_ONLY_ENEMY));
                break;
            case 107:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, ""));
                break;
            case 108:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BANZAI_BILL, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 109:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BLUE_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 110:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BOB_OMB, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 111:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BULLET_BILL, {x * tilewidth, y * tilewidth + 2}, Direction::LEFT));
                break;
            case 112:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BUZZY_BEETLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 113:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::FLYING_GOOMBA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 114:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::GOOMBA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 115:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::GREEN_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 116:
                backEnemy.push_back(EnemyFactory::createEnemy(EnemyType::JUMPING_PIRANHA_PLANT, {x * tilewidth + 16, y * tilewidth + 34}, Direction::LEFT));
                break;
            case 117:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MONTY_MOLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 118:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MUMMY_BEETLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 119:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MUNCHER, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 120:
                backEnemy.push_back(EnemyFactory::createEnemy(EnemyType::PIRANHA_PLANT, {x * tilewidth + 16, y * tilewidth + 32}, Direction::LEFT));
                break;
            case 121:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::RED_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 122:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::REX, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 123:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::SWOOPER, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 124:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::YELLOW_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 125:
                // cây ăn thịt xanh lá
                break;
            case 126:
                staticItem.push_back(ItemFactory::createItem(ItemType::COIN, Source::BLOCK, {x * tilewidth + 4, y * tilewidth}, Direction::UP));
                break;
            case 127:
                staticItem.push_back(ItemFactory::createItem(ItemType::COURSE_CLEAR_TOKEN, Source::BLOCK, {x * tilewidth - tilewidth, y * tilewidth}, Direction::UP));
                break;
            case 128:
                staticItem.push_back(ItemFactory::createItem(ItemType::YOSHI_COIN, Source::BLOCK, {x * tilewidth, y * tilewidth + 5}, Direction::UP));
                break;
            case 129:
                // dây leo cây ăn thịt xanh lá
                break;
            case 130:
                block.push_back(BlockFactory::createBlock(BlockType::CLOUDBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 131:
                block.push_back(BlockFactory::createBlock(BlockType::EXCLAMATIONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 132:
                block.push_back(BlockFactory::createBlock(BlockType::EYESCLOSED, {x * tilewidth, y * tilewidth}));
                break;
            case 133:
                block.push_back(BlockFactory::createBlock(BlockType::EYESOPENED, {x * tilewidth, y * tilewidth}));
                break;
            case 134:
                block.push_back(BlockFactory::createBlock(BlockType::GLASSBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 135:
                // mess block
                break;
            case 136:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 137:
                block.push_back(BlockFactory::createBlock(BlockType::STONEBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 138:
                block.push_back(BlockFactory::createBlock(BlockType::WOODBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 139:
                // item 1upmushroom
                break;
            case 140:
                // item 3upmoon
                break;
            case 141:
                // item fireflower
                break;
            case 142:
                // item mushroom
                break;
            case 143:
                // item star
                break;
            case 144:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONONEUPMUSHROOMBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 145:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONTHREEUPMOONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 146:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONFIREFLOWERBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 147:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONMUSHROOMBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 148:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONSTARBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 149:
                block.push_back(BlockFactory::createBlock(BlockType::INVISIBLEBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            default:
                std::cerr << "Unknown tile type: " << y * (width / tilewidth) + x << " at (" << x << ", " << y << ")" << std::endl;
                break;
            }
        }
    }
}

std::vector<int> Map::getMapGrid() const {
    return mapGrid;
}