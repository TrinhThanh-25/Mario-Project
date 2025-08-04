#ifndef CUSTOMMAPSTATE_H
#define CUSTOMMAPSTATE_H

#include "GameState/GameState.h"
#include "raylib.h"
#include <vector>
#include <string>
#include "raygui.h"

class CustomMapState : public GameState {
public:
    CustomMapState(World* world);

    void update() override;
    void draw() override;
    void enter() override;
    void exit() override;
private:  
    static const int TILE_SIZE = 32;
    int MAP_WIDTH = 35;
    int MAP_HEIGHT = 20;
    static const int MARGIN = 2;
    bool isEraseMode = false;
    bool isPaletteVisible = true;

    int selectedTileId = 0; // tile ID from 1 -> 149
    std::vector<std::vector<int>> mapGrid; // Lưu ID tile
    std::vector<std::string> tileIds; // Lưu key tương ứng trong ResourceManager 

    void loadTileTextures(); // Đổ tileIds từ Tileset1 đến Tileset149
    void drawMap() const;
    void drawTilePalette() const;
    void handleInput();

    int paletteCols = 11;
    int paletteScrollOffset = 0;

    bool drawButton(int x, int y, int w, int h, const char* label, int fontSize, Color hoverColor) const;
    std::map<std::string, std::vector<int>> tileCategories;
    std::string selectedCategory = "ALL";

    // FOR CUSTOM MAP
    int viewCols = 34;
    int viewRows = 20;

    Rectangle mapViewRect;
    Vector2 mapOffset = {30, 0};      // thay vì const mapOffsetX = 30
    Vector2 panOffset = {0, 0};      // Offset for panning the map
 
    bool isPanning = false;          // Flag to check if the user is panning
    bool isDragging = false;

    Vector2 mouseDownStart = {0, 0};
    float dragThreshold = 5.0f;

    void drawMapSizeInputUI();
    void resizeMap(int newWidth, int newHeight);

    void drawMapWithOffset() const; // Method to draw the map with panning offset

    bool isTypingWidth = false;
    bool isTypingHeight = false;

    char widthInput[6] = "50";
    char heightInput[6] = "20";

    Vector2 lastMouse = {0, 0};
    
};

#endif
