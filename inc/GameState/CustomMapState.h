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
    void setIsSaved(bool saved);
private:  
    std::string mapFileName;
    static const int TILE_SIZE = 32;
    int MAP_WIDTH;
    int MAP_HEIGHT;
    bool isEraseMode;
    bool isPaletteVisible;

    int selectedTileId;
    std::vector<int> mapGrid;
    std::vector<std::string> tileIds;
    Camera2D camera;
    bool isDragging;
    Vector2 lastMousePos;
    bool isDrawing;
    Vector2 lastDrawnTile;

    std::string mapNameBuffer;
    std::string widthBuffer;
    std::string heightBuffer;
    bool editingMapName;
    bool editingWidth;
    bool editingHeight;
    bool isClosed;
    bool isSaved;
    bool showUnsavedWarning;

    void loadTileTextures();
    void drawMap();
    void drawTilePalette();
    void drawToolbar();
    void updateTilePallete();
    void updateMapView();
    void saveMap();
    void loadMap();
    void clearMap();
    void applyMapSize();
    void handleUnsavedWarning();

    std::map<std::string, std::vector<int>> tileCategories;
    std::string selectedCategory = "ALL";
};

#endif
