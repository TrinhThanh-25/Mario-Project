#ifndef CUSTOMMAPSTATE_H
#define CUSTOMMAPSTATE_H

#include "GameState/GameState.h"
#include "raylib.h"
#include <vector>
#include <string>
#include <map>
#include "raygui.h"

class CustomMapState : public GameState {
public:
    CustomMapState(World* world, const std::string& mapFileName);
    CustomMapState(World* world, const std::string& mapFileName, int width, int height, const std::vector<int>& mapGrid);

    void update() override;
    void draw() override;
    void enter() override;
    void exit() override;
    void setMap(int width, int height, const std::vector<int>& mapGrid);
private:  
    std::string mapFileName;
    static const int TILE_SIZE = 32;
    int MAP_WIDTH = 35;
    int MAP_HEIGHT = 20;
    bool isEraseMode = false;
    bool isPaletteVisible = true;

    int selectedTileId = 0; // tile ID from 1 -> 149 ( 0 when erase mode )
    std::vector<int> mapGrid; // Lưu ID tile (1D vector, access via y * MAP_WIDTH + x)
    std::vector<std::string> tileIds; // Lưu key tương ứng trong ResourceManager 

    // Camera and view variables
    Camera2D camera;              // Raylib Camera2D
    bool isDragging = false;      // For panning
    Vector2 lastMousePos = {0, 0};
    bool isDrawing = false;       // For continuous tile drawing
    Vector2 lastDrawnTile = {-1, -1}; // Last tile coordinates drawn

    // Toolbar variables
    std::string mapNameBuffer = "NewMap";
    std::string widthBuffer = "35";
    std::string heightBuffer = "20";
    bool editingMapName = false;
    bool editingWidth = false;
    bool editingHeight = false;
    bool isClosed = false;
    bool isSaved = true;

    void loadTileTextures(); // Đổ tileIds từ Tileset1 đến Tileset149
    void drawMap();
    void drawTilePalette();
    void drawToolbar();
    void updateTilePallete();
    void updateMapView();
    void saveMap();
    void loadMap();
    void clearMap();
    void applyMapSize();

    std::map<std::string, std::vector<int>> tileCategories;
    std::string selectedCategory = "ALL";
};

#endif
