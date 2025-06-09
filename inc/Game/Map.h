#ifndef MAP_H
#define MAP_H

#include "raylib.h"
#include "Common/Drawable.h"
#include "Tile/Tile.h"
#include "Block/Block.h"
#include "Enemy/Enemy.h"
#include "Item/Item.h"

class World;
class Mario;

class Map : virtual public Drawable {
    private:
        Mario* mario;
        Camera2D* camera;
        World* world;

        int mapNumber;
        int width;
        int height;
        Texture2D background;
        int backgroundID;
        
        std::vector<Tile*> tile;
        std::vector<Tile*> backTile;
        std::vector<Tile*> frontTile;
        std::vector<Block*> block;
        std::vector<Block*> messBlock;
        std::vector<Enemy*> enemy;
        std::vector<Enemy*> backEnemy;
        std::vector<Enemy*> frontEnemy;
        std::vector<Item*> item;
    public:
        Map();
        ~Map() override;
        void loadMap(int mapnumber);
        void draw() override;
};

#endif