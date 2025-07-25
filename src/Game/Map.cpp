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
    // maxMapNumber(3),
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
    background = ResourceManager::getTexture()["Background1"];
    backgroundColor = WHITE;
    backgroundID = mapNumber;
    musicID = mapNumber;
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
	float tilewidth = mapJson["tilewidth"];
	std::vector<int> data = mapJson["layers"][0]["data"];
	for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            switch(data[y * width + x]){
            case 0:
                continue;
            case 112:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1A"));
                break;
            case 113:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1B"));
                break;
            case 114:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1C"));
                break;
            case 115:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1D"));
                break;
            case 116:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1E"));
                break;
            case 117:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1F"));
                break;
            case 118:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1G"));
                break;
            case 119:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1H"));
                break;
            case 120:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1I"));
                break;
            case 121:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1J"));
                break;
            case 122:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1K"));
                break;
            case 123:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1L"));
                break;
            case 124:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1M"));
                break;
            case 125:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1N"));
                break;
            case 126:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1O"));
                break;
            case 127:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1P"));
                break;
            case 128:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1Q"));
                break;
            case 129:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1R"));
                break;
            case 130:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1S"));
                break;
            case 131:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1T"));
                break;
            case 132:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1U"));
                break;
            case 133:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1V"));
                break;
            case 134:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1W"));
                break;
            case 135:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1X"));
                break;
            case 136:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1Y"));
                break;
            case 137:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "1Z"));
                break;
            case 138:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2A"));
                break;
            case 139:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2B"));
                break;
            case 140:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2C"));
                break;
            case 141:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2D"));
                break;
            case 142:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2E"));
                break;
            case 143:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2F"));
                break;
            case 144:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2G"));
                break;
            case 145:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2H"));
                break;
            case 146:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2I"));
                break;
            case 147:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2J"));
                break;
            case 148:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2K"));
                break;
            case 149:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2L"));
                break;
            case 150:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2M"));
                break;
            case 151:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2N"));
                break;
            case 152:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2O"));
                break;
            case 153:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2P"));
                break;
            case 154:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2Q"));
                break;
            case 155:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2R"));
                break;
            case 156:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2S"));
                break;
            case 157:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2T"));
                break;
            case 158:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2U"));
                break;
            case 159:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2V"));
                break;
            case 160:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2W"));
                break;
            case 161:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2X"));
                break;
            case 162:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2Y"));
                break;
            case 163:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "2Z"));
                break;
            case 164:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3A"));
                break;
            case 165:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3B"));
                break;
            case 166:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3C"));
                break;
            case 167:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3D"));
                break;
            case 168:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3E"));
                break;
            case 169:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3F"));
                break;
            case 170:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3G"));
                break;
            case 171:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3H"));
                break;
            case 172:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3I"));
                break;
            case 173:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3J"));
                break;
            case 174:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3K"));
                break;
            case 175:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3L"));
                break;
            case 176:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3M"));
                break;
            case 177:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3N"));
                break;
            case 178:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3O"));
                break;
            case 179:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3P"));
                break;
            case 180:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3Q"));
                break;
            case 181:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3R"));
                break;
            case 182:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3S"));
                break;
            case 183:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3T"));
                break;
            case 184:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3U"));
                break;
            case 185:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3V"));
                break;
            case 186:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3W"));
                break;
            case 187:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3X"));
                break;
            case 188:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3Y"));
                break;
            case 189:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "3Z"));
                break;
            case 190:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4A"));
                break;
            case 191:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4B"));
                break;
            case 192:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4C"));
                break;
            case 193:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4D"));
                break;
            case 194:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4E"));
                break;
            case 195:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4F"));
                break;
            case 196:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4G"));
                break;
            case 197:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4H"));
                break;
            case 198:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4I"));
                break;
            case 199:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4J"));
                break;
            case 200:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4K"));
                break;
            case 201:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4L"));
                break;
            case 202:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4M"));
                break;
            case 203:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4N"));
                break;
            case 204:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4O"));
                break;
            case 205:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4P"));
                break;
            case 206:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4Q"));
                break;
            case 207:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4R"));
                break;
            case 208:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4S"));
                break;
            case 209:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4T"));
                break;
            case 210:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4U"));
                break;
            case 211:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4V"));
                break;
            case 212:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4W"));
                break;
            case 213:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4X"));
                break;
            case 214:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4Y"));
                break;
            case 215:
                tile.push_back(TileFactory::createTile({x * tilewidth, y * tilewidth}, "4Z"));
                break;
            case 252:
            //
                break;
            case 253:
            //
                break;
            case 254:
            //
                break;
            case 255:
            //
                break;
            case 256:
                block.push_back(BlockFactory::createBlock(BlockType::CLOUDBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 257:
                block.push_back(BlockFactory::createBlock(BlockType::EXCLAMATIONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 258:
                block.push_back(BlockFactory::createBlock(BlockType::EYESCLOSED, {x * tilewidth, y * tilewidth}));
                break;
            case 259:
                block.push_back(BlockFactory::createBlock(BlockType::EYESOPENED, {x * tilewidth, y * tilewidth}));
                break;
            case 260:
                block.push_back(BlockFactory::createBlock(BlockType::GLASSBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 261:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 262:
                block.push_back(BlockFactory::createBlock(BlockType::STONEBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 263:
                block.push_back(BlockFactory::createBlock(BlockType::WOODBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 264:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONONEUPMUSHROOMBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 265:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONTHREEUPMOONBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 266:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONFIREFLOWERBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 267:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONMUSHROOMBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 268:
                block.push_back(BlockFactory::createBlock(BlockType::QUESTIONSTARBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 269:
                block.push_back(BlockFactory::createBlock(BlockType::INVISIBLEBLOCK, {x * tilewidth, y * tilewidth}));
                break;
            case 270:
                block.push_back(BlockFactory::createBlock(BlockType::MESSAGEBLOCK, {x * tilewidth, y * tilewidth}));//
                break;
            case 271:
                staticItem.push_back(ItemFactory::createItem(ItemType::COIN, Source::BLOCK, {x * tilewidth + 4, y * tilewidth}, Direction::UP));
                break;
            case 272:
                staticItem.push_back(ItemFactory::createItem(ItemType::COURSE_CLEAR_TOKEN, Source::BLOCK, {x * tilewidth - tilewidth, y * tilewidth}, Direction::UP));
                break;
            case 273:
                staticItem.push_back(ItemFactory::createItem(ItemType::YOSHI_COIN, Source::BLOCK, {x * tilewidth, y * tilewidth + 5}, Direction::UP));
                break;
            case 274:
                backEnemy.push_back(EnemyFactory::createEnemy(EnemyType::JUMPING_PIRANHA_PLANT, {x * tilewidth + 16, y * tilewidth + 34}, Direction::LEFT));
                break;
            case 275:
                backEnemy.push_back(EnemyFactory::createEnemy(EnemyType::PIRANHA_PLANT, {x * tilewidth + 16, y * tilewidth + 36}, Direction::LEFT));
                break;
            case 276:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BANZAI_BILL, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 277:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BLUE_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 278:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BOB_OMB, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 279:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BULLET_BILL, {x * tilewidth, y * tilewidth + 2}, Direction::LEFT));
                break;
            case 280:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::BUZZY_BEETLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 281:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::FLYING_GOOMBA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 282:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::GOOMBA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 283:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::GREEN_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 284:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MONTY_MOLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 285:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MUMMY_BEETLE, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 286:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::MUNCHER, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 287:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::RED_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 288:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::REX, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 289:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::SWOOPER, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 290:
                frontEnemy.push_back(EnemyFactory::createEnemy(EnemyType::YELLOW_KOOPA_TROOPA, {x * tilewidth, y * tilewidth}, Direction::LEFT));
                break;
            case 291:
                for (auto& character : characters) {
                    character->setPosition({x * tilewidth, y * tilewidth});
                }
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
    background = ResourceManager::getTexture()["Background" + std::to_string(backgroundID)];
    DrawRectangle(0, 0, width, height, backgroundColor);
    int repeat = width / background.width + 2;
    if ( backgroundID > 0 ) {
        for ( int i = 0; i <= repeat; i++ ) {
            DrawTexture(background,-background.width + i * background.width + offset * 0.5, height - background.height, WHITE );
        }
    }
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
    clear();
    StopMusicStream(ResourceManager::getMusic()["Invincible"]);
    StopMusicStream(ResourceManager::getMusic()["Music" + std::to_string(musicID)]);
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

json Map::saveToJson() const {
    json j;
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