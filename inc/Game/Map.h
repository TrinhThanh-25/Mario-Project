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
        int maxMapNumber;
        int width;
        int height;
        Texture2D background;
        Color backgroundColor;
        int backgroundID;
        int musicID;
        
        std::vector<Tile*> tile;
        std::vector<Tile*> backTile;
        std::vector<Tile*> frontTile;
        std::vector<Block*> block;
        std::vector<Block*> messBlock;
        std::vector<Enemy*> backEnemy;
        std::vector<Enemy*> frontEnemy;
        std::vector<Item*> item;
        std::vector<Item*> staticItem;
    public:
        Map(std::vector<Character*>& characters, World* world, int mapNumber);
        ~Map() override;
        void loadMap(int mapnumber);
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

        void showMessage();

        void clear();

        void reset();
        bool next();
        void first();
  
        void playMusic();

        json saveToJson() const;
        void loadFromJson(const json& j);
};

#endif