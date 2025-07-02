#include "Game/Map.h"
#include "Character/Character.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

Map::Map(std::vector<Character*>& characters, World* world, int mapNumber)
    : characters(characters), 
    world(world), 
    mapNumber(mapNumber), 
    offset(0.0f), 
    camera(nullptr), 
    width(0), 
    height(0),
    background(Texture2D()),
    backgroundColor(WHITE),
    backgroundID(0),
    musicID(0){
    loadMap(mapNumber);
}

Map::~Map() {
    clear();
}

void Map::loadMap(int mapNumber) {
    char* map = nullptr;
    std::string mapFileName = "../resources/Map/map" +std::to_string(mapNumber) + ".json";
    std::ifstream file(mapFileName);
	if (!file) {
		std::cerr << "Could not open json file " << mapFileName << std::endl;
		return;
	}
	nlohmann::json mapJson;
	file >> mapJson;
	int width = mapJson["width"];
	int height = mapJson["height"];
	this->width = (float) width * 32.0f;
    this->height = (float) height * 32.0f;
	int tilewidth = mapJson["tilewidth"];
	std::vector<int> data = mapJson["layers"][0]["data"];
	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch(data[y * width + x]){
            case 1:
                continue;
            case 112:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1A"));
                break;
            case 113:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1B"));
                break;
            case 114:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1C"));
                break;
            case 115:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1D"));
                break;
            case 116:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1E"));
                break;
            case 117:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1F"));
                break;
            case 118:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1G"));
                break;
            case 119:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1H"));
                break;
            case 120:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1I"));
                break;
            case 121:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1J"));
                break;
            case 122:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1K"));
                break;
            case 123:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1L"));
                break;
            case 124:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1M"));
                break;
            case 125:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1N"));
                break;
            case 126:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1O"));
                break;
            case 127:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1P"));
                break;
            case 128:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1Q"));
                break;
            case 129:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1R"));
                break;
            case 130:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1S"));
                break;
            case 131:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1T"));
                break;
            case 132:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1U"));
                break;
            case 133:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1V"));
                break;
            case 134:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1W"));
                break;
            case 135:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1X"));
                break;
            case 136:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1Y"));
                break;
            case 137:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "1Z"));
                break;
            case 138:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2A"));
                break;
            case 139:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2B"));
                break;
            case 140:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2C"));
                break;
            case 141:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2D"));
                break;
            case 142:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2E"));
                break;
            case 143:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2F"));
                break;
            case 144:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2G"));
                break;
            case 145:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2H"));
                break;
            case 146:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2I"));
                break;
            case 147:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2J"));
                break;
            case 148:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2K"));
                break;
            case 149:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2L"));
                break;
            case 150:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2M"));
                break;
            case 151:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2N"));
                break;
            case 152:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2O"));
                break;
            case 153:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2P"));
                break;
            case 154:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2Q"));
                break;
            case 155:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2R"));
                break;
            case 156:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2S"));
                break;
            case 157:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2T"));
                break;
            case 158:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2U"));
                break;
            case 159:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2V"));
                break;
            case 160:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2W"));
                break;
            case 161:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2X"));
                break;
            case 162:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2Y"));
                break;
            case 163:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "2Z"));
                break;
            case 164:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3A"));
                break;
            case 165:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3B"));
                break;
            case 166:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3C"));
                break;
            case 167:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3D"));
                break;
            case 168:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3E"));
                break;
            case 169:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3F"));
                break;
            case 170:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3G"));
                break;
            case 171:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3H"));
                break;
            case 172:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3I"));
                break;
            case 173:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3J"));
                break;
            case 174:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3K"));
                break;
            case 175:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3L"));
                break;
            case 176:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3M"));
                break;
            case 177:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3N"));
                break;
            case 178:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3O"));
                break;
            case 179:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3P"));
                break;
            case 180:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3Q"));
                break;
            case 181:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3R"));
                break;
            case 182:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3S"));
                break;
            case 183:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3T"));
                break;
            case 184:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3U"));
                break;
            case 185:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3V"));
                break;
            case 186:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3W"));
                break;
            case 187:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3X"));
                break;
            case 188:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3Y"));
                break;
            case 189:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "3Z"));
                break;
            case 190:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4A"));
                break;
            case 191:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4B"));
                break;
            case 192:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4C"));
                break;
            case 193:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4D"));
                break;
            case 194:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4E"));
                break;
            case 195:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4F"));
                break;
            case 196:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4G"));
                break;
            case 197:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4H"));
                break;
            case 198:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4I"));
                break;
            case 199:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4J"));
                break;
            case 200:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4K"));
                break;
            case 201:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4L"));
                break;
            case 202:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4M"));
                break;
            case 203:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4N"));
                break;
            case 204:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4O"));
                break;
            case 205:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4P"));
                break;
            case 206:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4Q"));
                break;
            case 207:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4R"));
                break;
            case 208:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4S"));
                break;
            case 209:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4T"));
                break;
            case 210:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4U"));
                break;
            case 211:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4V"));
                break;
            case 212:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4W"));
                break;
            case 213:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4X"));
                break;
            case 214:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4Y"));
                break;
            case 215:
                tile.push_back(new Tile({(float)x * tilewidth, (float)y * tilewidth}, {32.0f, 32.0f}, "4Z"));
                break;
            default:
                std::cerr << "Unknown tile type: " << data[y * width + x] << " at (" << x << ", " << y << ")" << std::endl;
                break;
            }
        }
	}
}

void Map::setCharacters(std::vector<Character*>& characters) {
    this->characters = characters;
}

void Map::draw() {
    DrawRectangle(0, 0, width, height, backgroundColor);
    // int repeats = width / background.width+2;
    // if ( backgroundID > 0 ) {
    //     for ( int i = 0; i <= repeats; i++ ) {
    //         DrawTexture(background,-background.width + i * background.width + offset * 0.4, height - background.height, WHITE );
    //     }
    // }
    for (auto& t : tile) {
        t->draw();
    }
    for (auto& bT : backTile) {
        bT->draw();
    }
    for (auto& b : block) {
        b->draw();
    }
    for (auto& mB : messBlock) {
        mB->draw();
    }
    for (auto& bE : backEnemy) {
        bE->draw();
    }
    for (auto& i : item) {
        i->draw();
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
        delete t;
    }
    tile.clear();
    for (auto& bT : backTile) {
        delete bT;
    }
    backTile.clear();
    for (auto& fT : frontTile) {
        delete fT;
    }
    frontTile.clear();
    for (auto& b : block) {
        delete b;
    }
    block.clear();
    for (auto& mB : messBlock) {
        delete mB;
    }
    messBlock.clear();
    for (auto& bE : backEnemy) {
        delete bE;
    }
    backEnemy.clear();
    for (auto& fE : frontEnemy) {
        delete fE;
    }
    frontEnemy.clear();
    for (auto& i : item) {
        delete i;
    }
    item.clear();
}

void Map::showMessage() {
    // Hiện thông báo nếu có
}

void Map::reset() {
    // thêm vẽ vẽ với reset mặc định

    clear();
    loadMap(mapNumber);
}

bool Map::next() {
    if (mapNumber < maxMapNumber) {
        mapNumber++;
        reset();
        return true;
    }
    return false;
}

void Map::first() {
    mapNumber = 1;
    reset();
}