#include "Game/Map.h"
#include "json.hpp"
#include <fstream>
#include <iostream>

Map::Map(){
    
}

Map::~Map() {
    for (auto& t : tile) {
        delete t;
    }
    for (auto& bT : backTile) {
        delete bT;
    }
    for (auto& fT : frontTile) {
        delete fT;
    }
    for (auto& b : block) {
        delete b;
    }
    for (auto& mB : messBlock) {
        delete mB;
    }
    for (auto& e : enemy) {
        delete e;
    }
    for (auto& bE : backEnemy) {
        delete bE;
    }
    for (auto& fE : frontEnemy) {
        delete fE;
    }
    for (auto& i : item) {
        delete i;
    }
}

void Map::loadMap(int mapNumber)
{
    char* map = nullptr;
    std::string mapFileName = "resources/Map/M" +std::to_string(mapNumber) + ".json";
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
                case 0:
                    continue;
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                    
                    break;
            }
            
            // else if(tileId==1)
            // 	nonInterativeTile.push_back(new Tile(Vector2{(float) x * 32,(float) y * 32 },mapNumber,tileId-1));
            // else interactiveTiles.push_back(new Tile(Vector2{(float) x * 32,(float) y * 32 },mapNumber,tileId-1));
		}
	}
}

void Map::draw() {
    // drawTexture(background, 0, 0, WHITE);
    for (auto& t : tile) {
        t->draw();
    }
    for (auto& bT : backTile) {
        bT->draw();
    }
    for (auto& fT : frontTile) {
        fT->draw();
    }
    for (auto& b : block) {
        b->draw();
    }
    for (auto& mB : messBlock) {
        mB->draw();
    }
    for (auto& e : enemy) {
        e->draw();
    }
    for (auto& bE : backEnemy) {
        bE->draw();
    }
    for (auto& fE : frontEnemy) {
        fE->draw();
    }
    for (auto& i : item) {
        i->draw();
    }
}