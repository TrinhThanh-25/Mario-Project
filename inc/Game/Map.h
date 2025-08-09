#ifndef MAP_H
#define MAP_H

#include <vector>
#include "raylib.h"
#include "Common/Drawable.h"
#include "json.hpp"

using json = nlohmann::json;

class World;
class Character;
class Tile;
class Block;
class Enemy;
class Item;

class Map : virtual public Drawable {
    private:
        std::vector<Character*>& characters;
        float offset;
        Camera2D* camera;
        World* world;

        int mapNumber;
        std::string mapFileName;
        int maxMapNumber;
        int width;
        int height;
        Texture2D background;
        Color backgroundColor;
        int backgroundID;
        int musicID;
        
        std::vector<int> mapGrid; 
        std::vector<Tile*> tile;
        std::vector<Tile*> backTile;
        std::vector<Tile*> frontTile;
        std::vector<Block*> block;
        std::vector<Block*> messBlock;
        std::vector<Enemy*> backEnemy;
        std::vector<Enemy*> frontEnemy;
        std::vector<Item*> item;
        std::vector<Item*> staticItem;
        bool netMode = false;
    public:
        Map(std::vector<Character*>& characters, World* world, int mapNumber);
        ~Map() override;
        void loadMap(int mapnumber);
        void loadMap(const std::string& mapFileName);
        void setCharacters(std::vector<Character*>& characters);
        void draw() override;
        void setOffset(float offset);
        void setCamera(Camera2D* camera);
        int getWidth() const;
        int getHeight() const;

        std::vector<Tile*>& getTile();
        std::vector<Block*>& getBlock();
        std::vector<Enemy*>& getBackEnemy();
        std::vector<Enemy*>& getFrontEnemy();
        std::vector<Item*>& getItem();
        std::vector<Item*>& getStaticItem();

        void clear();

        void reset(bool isTestMap = false);
        bool next();
        void first();
  
        void playMusic();
        void stopMusic();

        json saveToJson() const;
        void loadFromJson(const json& j);

        void setNetMode(bool net);
        std::string getMapFileName() const;
        void setMapFileName(const std::string& mapFileName);
        void setMap(int width, int height, const std::vector<int>& mapGrid);
        void loadObjectsToMap(const std::vector<int>& mapGrid);
        std::vector<int> getMapGrid() const;
};

#endif