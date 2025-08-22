#include "GameState/CustomMapState.h"
#include "GameState/TestMapState.h"
#include "Common/ResourceManager.h"
#include "GameState/ChooseCustomizedMap.h"
#include "GameState/GameStateFactory.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdlib>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;

// CONST FOR LATER
const int PALETTE_WIDTH = 290;
const int BUTTON_HEIGHT = 30;
const int CATEGORY_BUTTON_WIDTH = 50;
const int TOGGLE_BUTTON_WIDTH = 30;
const int TILE_DISPLAY_SIZE = 24;
const int TILE_SPACING = 2;
const int TOOLBAR_HEIGHT = 40;
const int TOOLBAR_BUTTON_WIDTH = 80;
const int TOOLBAR_TEXTBOX_WIDTH = 100;
const int TOOLBAR_SPACING = 10;

CustomMapHistory::CustomMapHistory()
    : currentIndex(-1) {}

void CustomMapHistory::addMemento(const CustomMapMemento& memento) {
    if (currentIndex < (history.size()) - 1) {
        history.erase(history.begin() + currentIndex + 1, history.end());
    }
    history.push_back(memento);
    currentIndex++;
};

CustomMapMemento CustomMapHistory::getMemento() const {
    if (currentIndex >= 0 && currentIndex < history.size()) {
        return history[currentIndex];
    }
    throw std::out_of_range("No memento available at current index.");
}

void CustomMapHistory::undo() {
    if (canUndo()) {
        currentIndex--;
    }
}

void CustomMapHistory::redo() {
    if (canRedo()) {
        currentIndex++;
    }
}

bool CustomMapHistory::canUndo() const {
    return currentIndex > 0;
}

bool CustomMapHistory::canRedo() const {
    return currentIndex < (history.size()) - 1;
}

void CustomMapHistory::clear() {
    history.clear();
    currentIndex = -1;
}

std::vector<std::string> getCategories() {
    return {"ALL", "TILE", "ITEM", "BLOCK", "ENEMY"};
}

int getToggleX(bool isPaletteVisible) {
    return isPaletteVisible ? GetScreenWidth() - PALETTE_WIDTH - 5 : GetScreenWidth() - TOGGLE_BUTTON_WIDTH - 5;
}

CustomMapState::CustomMapState(World* world)
    : GameState(world, GameStateType::CUSTOM_MAP),
      MAP_WIDTH(35), MAP_HEIGHT(20),
      isEraseMode(false), isPaletteVisible(true),
      selectedTileId(0),
      isDragging(false), isDrawing(false),
      lastMousePos({0, 0}), lastDrawnTile({-1, -1}),
      editingMapName(false), editingWidth(false), editingHeight(false),
      isClosed(false), isSaved(true), showUnsavedWarning(false),
      camera(world->getCamera()) {
    loadTileTextures();
    widthBuffer = std::to_string(MAP_WIDTH);
    heightBuffer = std::to_string(MAP_HEIGHT);
}

CustomMapState::CustomMapState(World* world, const std::string& mapFileName)
    : CustomMapState(world) {
    setMapFileName(mapFileName);
}

void CustomMapState::setMapFileName(const std::string& mapFileName) {
    this->mapFileName = mapFileName;
    loadMap();
    mapNameBuffer = mapFileName;
}

CustomMapState::CustomMapState(World* world, const std::string& mapFileName, int width, int height, const std::vector<int>& mapGrid)
    : GameState(world, GameStateType::CUSTOM_MAP),
      mapFileName(mapFileName), MAP_WIDTH(width), MAP_HEIGHT(height),
      isEraseMode(false), isPaletteVisible(true),
      selectedTileId(0),
      isDragging(false), isDrawing(false),
      lastMousePos({0, 0}), lastDrawnTile({-1, -1}),
      editingMapName(false), editingWidth(false), editingHeight(false),
        isClosed(false), isSaved(true), showUnsavedWarning(false),
        camera(world->getCamera()) {
    loadTileTextures();
    this->mapGrid = mapGrid;
    mapNameBuffer = mapFileName;
    widthBuffer = std::to_string(MAP_WIDTH);
    heightBuffer = std::to_string(MAP_HEIGHT);
}

void CustomMapState::enter() {
    GuiSetStyle(DEFAULT, TEXT_SIZE, 10);
}

void CustomMapState::exit() {}

void CustomMapState::handleUnsavedWarning() {
    if (!showUnsavedWarning) return;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0, 0, 0, 128});
    int dialogWidth = 400;
    int dialogHeight = 200;
    int dialogX = (GetScreenWidth() - dialogWidth) / 2;
    int dialogY = (GetScreenHeight() - dialogHeight) / 2;
    Rectangle dialogRect = {(float)dialogX, (float)dialogY, (float)dialogWidth, (float)dialogHeight};
    DrawRectangleRec(dialogRect, WHITE);
    DrawRectangleLinesEx(dialogRect, 2, BLACK);
 
    std::string warningText = "Unsaved Changes!";
    std::string messageText = "You have unsaved changes.\nDo you want to save before exiting?";
    
    DrawText(warningText.c_str(), dialogX + 20, dialogY + 20, 20, RED);
    DrawText(messageText.c_str(), dialogX + 20, dialogY + 60, 16, BLACK);

    Rectangle saveBtn = {(float)(dialogX + 20), (float)(dialogY + 140), 100, 30};
    Rectangle discardBtn = {(float)(dialogX + 140), (float)(dialogY + 140), 100, 30};
    Rectangle cancelBtn = {(float)(dialogX + 260), (float)(dialogY + 140), 100, 30};
    
    // =============================== DRAW BUTTON ==============================
    if (GuiButton(saveBtn, "SAVE & EXIT")) {
        saveMap();
        showUnsavedWarning = false;
        isClosed = true;
        pendingExit = true;
        nextState = GameStateType::CHOOSE_CUSTOMIZED_MAP;
        return; 
    } else if (GuiButton(discardBtn, "DISCARD")) {
        showUnsavedWarning = false;
        isClosed = true;
        pendingExit = true;
        nextState = GameStateType::CHOOSE_CUSTOMIZED_MAP;
        return;
    } else if (GuiButton(cancelBtn, "CANCEL")) {
        showUnsavedWarning = false;
    }
}

void CustomMapState::updateTilePallete() {
    Vector2 mouse = GetMousePosition();
    
    if (!isPaletteVisible) return;

    std::vector<std::string> categories = getCategories();
    int paletteX = GetScreenWidth() - PALETTE_WIDTH;
    int paletteY = 40;
    int categoryY = paletteY + 10;
    int tileAreaY = categoryY + BUTTON_HEIGHT + 15;
    int tilesPerRow = (PALETTE_WIDTH - 20) / (TILE_DISPLAY_SIZE + TILE_SPACING);
    
    std::vector<int> drawList;
    if (selectedCategory == "ALL") {
        for (int i = 1; i <= 153; i++) {  // Fixed: loop to 153 to include all tiles
            drawList.push_back(i);
        }
    } else if (tileCategories.find(selectedCategory) != tileCategories.end()) {
        drawList = tileCategories.at(selectedCategory);
    }
    
    for (int i = 0; i < drawList.size(); i++) {
        int col = i % tilesPerRow;
        int row = i / tilesPerRow;
        
        int tileX = paletteX + 10 + col * (TILE_DISPLAY_SIZE + TILE_SPACING);
        int tileY = tileAreaY + row * (TILE_DISPLAY_SIZE + TILE_SPACING);
        
        Rectangle tileDest = {(float)tileX, (float)tileY, (float)TILE_DISPLAY_SIZE, (float)TILE_DISPLAY_SIZE};
        
        if (CheckCollisionPointRec(mouse, tileDest) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            selectedTileId = drawList[i];
            isEraseMode = false;
        }
    }
}

void CustomMapState::update() {
    if (pendingExit) {
        pendingExit = false;
        if (pendingTestState) {
            world->setGameState(pendingTestState.release()); // chuyển ownership
        } else if (nextState == GameStateType::CHOOSE_CUSTOMIZED_MAP) {
            world->setGameState(GameStateFactory::createGameState(world, GameStateType::CHOOSE_CUSTOMIZED_MAP));
        }
        return; // dừng update frame này
    }
    if (showUnsavedWarning) return;
    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Z)) {
        undo();
    } else if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_Y)) {
        redo();
    }
    updateTilePallete();
    updateMapView();
}

void CustomMapState::updateMapView() {
    Vector2 mouse = GetMousePosition();
    
    int paletteWidth = isPaletteVisible ? PALETTE_WIDTH : 0;
    Rectangle mapViewArea = {0, (float)TOOLBAR_HEIGHT, (float)(GetScreenWidth() - paletteWidth), (float)(GetScreenHeight() - TOOLBAR_HEIGHT)};
    
    camera->offset = {mapViewArea.width / 2.0f, mapViewArea.height / 2.0f + TOOLBAR_HEIGHT};
    
    if (CheckCollisionPointRec(mouse, mapViewArea)) {
        if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && GetMouseWheelMove() != 0) {
            float wheelMove = GetMouseWheelMove();
            Vector2 mouseWorldPos = GetScreenToWorld2D(mouse, *camera);
            camera->zoom = std::clamp(camera->zoom + wheelMove * 0.1f, 0.1f, 5.0f);
            Vector2 mouseWorldPosAfter = GetScreenToWorld2D(mouse, *camera);
            camera->target.x += mouseWorldPos.x - mouseWorldPosAfter.x;
            camera->target.y += mouseWorldPos.y - mouseWorldPosAfter.y;
        }
        float wheelMove = GetMouseWheelMove();
        if (wheelMove != 0.0f && !IsKeyDown(KEY_LEFT_CONTROL) && !IsKeyDown(KEY_RIGHT_CONTROL)) {
            if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT)) {
                camera->target.x += wheelMove * 50.0f / camera->zoom;
            } else {
                camera->target.y -= wheelMove * 50.0f / camera->zoom;
            }
        }
        if ((IsKeyDown(KEY_SPACE) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) && !isDragging) {
            isDragging = true;
            lastMousePos = mouse;
        }
        
        if (isDragging) {
            if ((IsKeyDown(KEY_SPACE) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
                Vector2 delta = {mouse.x - lastMousePos.x, mouse.y - lastMousePos.y};
                camera->target.x -= delta.x / camera->zoom;
                camera->target.y -= delta.y / camera->zoom;
                lastMousePos = mouse;
            } else {
                isDragging = false;
            }
        }
        
        // Handle tile placement and continuous drawing
        if (!IsKeyDown(KEY_SPACE) && (selectedTileId > 0 || isEraseMode)) {
            // Start drawing on left mouse press
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !isDragging) {
                isDrawing = true;
                
                // Place first tile
                Vector2 worldPos = GetScreenToWorld2D(mouse, *camera);
                int tileX = (int)(worldPos.x / TILE_SIZE);
                int tileY = (int)(worldPos.y / TILE_SIZE);
                
                if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
                    int newTileId = isEraseMode ? 0 : selectedTileId;
                    if (mapGrid[tileY * MAP_WIDTH + tileX] != newTileId) {
                        mapGrid[tileY * MAP_WIDTH + tileX] = newTileId;
                        lastDrawnTile = {(float)tileX, (float)tileY};
                        isSaved = false;
                    }
                }
            }
            
            // Continue drawing while mouse is held down
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && isDrawing && !isDragging) {
                Vector2 worldPos = GetScreenToWorld2D(mouse, *camera);
                int tileX = (int)(worldPos.x / TILE_SIZE);
                int tileY = (int)(worldPos.y / TILE_SIZE);
                
                // Only draw if we're on a different tile than last time
                if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT && 
                    (tileX != (int)lastDrawnTile.x || tileY != (int)lastDrawnTile.y)) {
                    int newTileId = isEraseMode ? 0 : selectedTileId;
                    if (mapGrid[tileY * MAP_WIDTH + tileX] != newTileId) {
                        mapGrid[tileY * MAP_WIDTH + tileX] = newTileId;
                        lastDrawnTile = {(float)tileX, (float)tileY};
                        isSaved = false;
                    }
                }
            }
            
            // Stop drawing when mouse is released
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                if (isDrawing) {
                    saveHistory();
                    isDrawing = false;
                }
                lastDrawnTile = {-1, -1};
            }
        }
    }
}

void CustomMapState::draw() {
    if (isClosed) return;
    ClearBackground({230, 230, 230, 255});
    drawMap();
    drawToolbar();
    if(isClosed) {
        return;
    }
    drawTilePalette();
    handleUnsavedWarning();
}

void CustomMapState::drawMap() {
    int paletteWidth = isPaletteVisible ? PALETTE_WIDTH : 0;
    Rectangle mapViewArea = {0, (float)TOOLBAR_HEIGHT, (float)(GetScreenWidth() - paletteWidth), (float)(GetScreenHeight() - TOOLBAR_HEIGHT)};
    DrawRectangleRec(mapViewArea, {200, 200, 200, 255});
    BeginMode2D(*camera);
    float mapWorldWidth = MAP_WIDTH * TILE_SIZE;
    float mapWorldHeight = MAP_HEIGHT * TILE_SIZE;
    Rectangle mapRect = {0, 0, mapWorldWidth, mapWorldHeight};
    DrawRectangleRec(mapRect, {245, 245, 245, 255});
    DrawRectangleLinesEx(mapRect, 2, DARKBLUE);
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int tileId = mapGrid[y * MAP_WIDTH + x];
            if (tileId > 0 && tileId <= (int)tileIds.size()) {
                const std::string& key = tileIds[tileId - 1];
                Texture2D tex = ResourceManager::getTexture()[key];
                
                float tileX = x * TILE_SIZE;
                float tileY = y * TILE_SIZE + TILE_SIZE - tex.height;
                
                DrawTexture(tex, tileX, tileY, WHITE);
            }
        }
    }
    for (int x = 0; x <= MAP_WIDTH; ++x) {
        float lineX = x * TILE_SIZE;
        DrawLine(lineX, 0, lineX, mapWorldHeight, BLACK);
    }
    for (int y = 0; y <= MAP_HEIGHT; ++y) {
        float lineY = y * TILE_SIZE;
        DrawLine(0, lineY, mapWorldWidth, lineY, BLACK);
    }
    
    Vector2 mouse = GetMousePosition();
    if (CheckCollisionPointRec(mouse, mapViewArea) && (selectedTileId > 0 || isEraseMode) && 
        !IsKeyDown(KEY_SPACE) && !isDragging) {
        Vector2 worldPos = GetScreenToWorld2D(mouse, *camera);
        int tileX = (int)(worldPos.x / TILE_SIZE);
        int tileY = (int)(worldPos.y / TILE_SIZE);
        
        if (tileX >= 0 && tileX < MAP_WIDTH && tileY >= 0 && tileY < MAP_HEIGHT) {
            if (!isEraseMode && selectedTileId > 0 && selectedTileId <= (int)tileIds.size()) {
                const std::string& key = tileIds[selectedTileId - 1];
                Texture2D tex = ResourceManager::getTexture()[key];
                
                float previewX = tileX * TILE_SIZE;
                float previewY = tileY * TILE_SIZE + TILE_SIZE - tex.height;
                
                DrawTexture(tex, previewX, previewY, Color{255, 255, 255, 150});
            }
            Rectangle tileRect = {(float)(tileX * TILE_SIZE), (float)(tileY * TILE_SIZE), TILE_SIZE, TILE_SIZE};
            Color borderColor = isEraseMode ? RED : GREEN;
            DrawRectangleLinesEx(tileRect, 2, borderColor);
        }
    }
    EndMode2D();
    
    std::string zoomText = "Zoom: " + std::to_string(static_cast<int>(camera->zoom * 100)) + "%";
    DrawText(zoomText.c_str(), 10, TOOLBAR_HEIGHT + 10, 16, BLACK);
    DrawText("Ctrl+Wheel: Zoom", 10, TOOLBAR_HEIGHT + 30, 12, DARKGRAY);
    DrawText("Space+Drag: Pan", 10, TOOLBAR_HEIGHT + 50, 12, DARKGRAY);
    DrawText("Left Click+Drag: Draw", 10, TOOLBAR_HEIGHT + 70, 12, DARKGRAY);
    
    std::string modeText = isEraseMode ? "Erase Mode" : "Draw Mode";
    Color modeColor = isEraseMode ? RED : GREEN;
    DrawText(modeText.c_str(), 10, TOOLBAR_HEIGHT + 90, 14, modeColor);

    std::string saveText = isSaved ? "Map Saved" : "Map Not Saved";
    Color saveColor = isSaved ? GREEN : RED;
    DrawText(saveText.c_str(), 10, TOOLBAR_HEIGHT + 110, 14, saveColor);
}

void CustomMapState::drawTilePalette() {
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();
    // Có thể kiếm texture để thay thế sau
    Rectangle toggleButton = {(float)getToggleX(isPaletteVisible), 5, (float)TOGGLE_BUTTON_WIDTH, (float)BUTTON_HEIGHT};
    if (GuiButton(toggleButton, isPaletteVisible ? "<<" : ">>")) {
        isPaletteVisible = !isPaletteVisible;
    }
    
    if (!isPaletteVisible) return;
    int paletteX = GetScreenWidth() - PALETTE_WIDTH;
    int paletteY = 40;
    int paletteHeight = screenHeight - paletteY - 20;
    Rectangle palettePanel = {(float)paletteX, (float)paletteY, (float)PALETTE_WIDTH, (float)paletteHeight};
    DrawRectangleRec(palettePanel, {250, 250, 250, 255});
    DrawRectangleLinesEx(palettePanel, 2, DARKGRAY);
    std::vector<std::string> categories = getCategories();
    int categoryY = paletteY + 10;
    
    for (int i = 0; i < categories.size(); i++) {
        int categoryX = paletteX + 10 + i * (CATEGORY_BUTTON_WIDTH + 5);
        Rectangle categoryButton = {(float)categoryX, (float)categoryY, (float)CATEGORY_BUTTON_WIDTH, (float)BUTTON_HEIGHT};
        std::string buttonText = categories[i];
        bool isSelected = (selectedCategory == categories[i]);
        if (isSelected) {
            GuiSetState(STATE_PRESSED);
            GuiButton(categoryButton, buttonText.c_str());
            GuiSetState(STATE_NORMAL);
        } else {
            if (GuiButton(categoryButton, buttonText.c_str())) {
                selectedCategory = categories[i];
            }
        }
    }
    
    int tileAreaY = categoryY + BUTTON_HEIGHT + 15;
    int tileAreaHeight = paletteHeight - (tileAreaY - paletteY) - 10;
    int tilesPerRow = (PALETTE_WIDTH - 20) / (TILE_DISPLAY_SIZE + TILE_SPACING);
    int maxVisibleRows = tileAreaHeight / (TILE_DISPLAY_SIZE + TILE_SPACING);
// list to be drawn 
    std::vector<int> drawList;
    if (selectedCategory == "ALL") {
        for (int i = 1; i <= 153; i++) {  // Fixed: loop to 153 to include all tiles
            drawList.push_back(i);
        }
    } else if (tileCategories.find(selectedCategory) != tileCategories.end()) {
        drawList = tileCategories.at(selectedCategory);
    }

    for (int i = 0; i < drawList.size(); i++) {
        int col = i % tilesPerRow;
        int row = i / tilesPerRow;
        
        if (row >= maxVisibleRows) break;
        
        int tileX = paletteX + 10 + col * (TILE_DISPLAY_SIZE + TILE_SPACING);
        int tileY = tileAreaY + row * (TILE_DISPLAY_SIZE + TILE_SPACING);
        
        Rectangle tileDest = {(float)tileX, (float)tileY, (float)TILE_DISPLAY_SIZE, (float)TILE_DISPLAY_SIZE};
        
        int tileId = drawList[i];
        if (tileId > 0 && tileId <= (int)tileIds.size()) {
            const std::string& key = tileIds[tileId - 1];
            Texture2D tex = ResourceManager::getTexture()[key];
            
            Color tint = (selectedTileId == tileId) ? YELLOW : WHITE;
            Color borderColor = (selectedTileId == tileId) ? RED : GRAY;
            
            DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, tileDest, {0, 0}, 0.0f, tint);
            DrawRectangleLinesEx(tileDest, (selectedTileId == tileId) ? 2 : 1, borderColor);
        }
    }
}

// load texture ảnh theo ID để dễ vẽ
void CustomMapState::loadTileTextures() {
    for (int i = 1; i <= 153; ++i) {
        tileIds.push_back("Tileset" + std::to_string(i));
        if ((i >= 2 && i <= 107) || (i >= 150 && i <= 153)) {
            tileCategories["TILE"].push_back(i);
        }

        else if (i >= 108 && i <= 124){
            tileCategories["ENEMY"].push_back(i);
        }

        else if ((i >= 125 && i <= 129)|| (i >= 139 && i <= 143)) {
            tileCategories["ITEM"].push_back(i);
        }

        else if ((i >= 130 && i <= 138) || (i >= 144 && i <= 149)){
            tileCategories["BLOCK"].push_back(i);
        }
    }
}

void CustomMapState::drawToolbar() {
    Rectangle toolbarArea = {0, 0, (float)GetScreenWidth(), (float)TOOLBAR_HEIGHT};
    DrawRectangleRec(toolbarArea, {50, 50, 50, 255});
    
    float currentX = TOOLBAR_SPACING;

    // Return button
    Rectangle returnBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if ((GuiButton(returnBtn, "RETURN") || IsKeyPressed(KEY_ESCAPE)) && !showUnsavedWarning) {
        if (!isSaved) {
            showUnsavedWarning = true;
        } else {
            world->setGameState(GameStateFactory::createGameState(world, GameStateType::CHOOSE_CUSTOMIZED_MAP));
            isClosed = true;
        }
        return;
    }
    currentX += TOOLBAR_BUTTON_WIDTH + TOOLBAR_SPACING;
    
    // Save button
    Rectangle saveBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if (GuiButton(saveBtn, "SAVE")) {
        saveMap();
    }
    currentX += TOOLBAR_BUTTON_WIDTH + TOOLBAR_SPACING;
    
    // Test button
    Rectangle testBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if (GuiButton(testBtn, "TEST")) {
        isClosed = true;
        pendingExit = true;
    // Tạo state mới ở update() để tránh hủy giữa draw.
    // Lưu các thông tin cần đem sang:
        pendingTestState.reset(GameStateFactory::createGameState(world, GameStateType::TEST_MAP));
        pendingTestState->setMapFileName(mapFileName);
        pendingTestState->setMap(MAP_WIDTH, MAP_HEIGHT, mapGrid);
        pendingTestState->setIsSaved(isSaved);
        pendingTestState->setCameraPosition(camera->target.x, camera->target.y);
        return;
    }
    currentX += TOOLBAR_BUTTON_WIDTH + TOOLBAR_SPACING;
    
    // Clear button
    Rectangle clearBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if (GuiButton(clearBtn, "CLEAR")) {
        clearMap();
        saveHistory();
    }
    currentX += TOOLBAR_BUTTON_WIDTH + TOOLBAR_SPACING;
    
    // Erase toggle button
    Rectangle eraseBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if (GuiButton(eraseBtn, isEraseMode ? "#131#ERASE" : "ERASE")) {
        isEraseMode = !isEraseMode;
    }
    currentX += TOOLBAR_BUTTON_WIDTH + TOOLBAR_SPACING * 2;
    
    DrawText("Name:", currentX, 12, 14, WHITE);
    currentX += 45;
    
    // Resize string buffers to accommodate GuiTextBox requirements
    mapNameBuffer.resize(65, '\0');
    widthBuffer.resize(17, '\0');
    heightBuffer.resize(17, '\0');
    
    Rectangle nameBox = {currentX, 8, TOOLBAR_TEXTBOX_WIDTH, TOOLBAR_HEIGHT - 16};
    if (GuiTextBox(nameBox, const_cast<char*>(mapNameBuffer.data()), 64, editingMapName)) {
        editingMapName = !editingMapName;
        int len = strlen(mapNameBuffer.data());
        mapNameBuffer.resize(len);
        isSaved = false;
    }
    currentX += TOOLBAR_TEXTBOX_WIDTH + TOOLBAR_SPACING;
    
    // Width label and textbox
    DrawText("W:", currentX, 12, 14, WHITE);
    currentX += 20;
    Rectangle widthBox = {currentX, 8, 60, TOOLBAR_HEIGHT - 16};
    if (GuiTextBox(widthBox, const_cast<char*>(widthBuffer.data()), 16, editingWidth)) {
        editingWidth = !editingWidth;
        int len = strlen(widthBuffer.data());
        widthBuffer.resize(len);
    }
    currentX += 60 + TOOLBAR_SPACING;
    
    // Height label and textbox
    DrawText("H:", currentX, 12, 14, WHITE);
    currentX += 20;
    Rectangle heightBox = {currentX, 8, 60, TOOLBAR_HEIGHT - 16};
    if (GuiTextBox(heightBox, const_cast<char*>(heightBuffer.data()), 16, editingHeight)) {
        editingHeight = !editingHeight;
        int len = strlen(heightBuffer.data());
        heightBuffer.resize(len);
    }
    currentX += 60 + TOOLBAR_SPACING;
    
    // Apply button
    Rectangle applyBtn = {currentX, 5, TOOLBAR_BUTTON_WIDTH, TOOLBAR_HEIGHT - 10};
    if (GuiButton(applyBtn, "APPLY")) {
        applyMapSize();
        saveHistory();
    }
}

void CustomMapState::saveMap() {
    std::string cleanName = mapNameBuffer.c_str();
    if (cleanName.empty()) {
        std::cerr << "Cannot save map with empty filename" << std::endl;
        return;
    }
    bool nameChanged = (mapFileName != cleanName);
    if (nameChanged && isMapNameExists(cleanName)) {
        cleanName = generateUniqueMapName(cleanName);
        mapNameBuffer = cleanName;
    }
    if (nameChanged) {
        std::string oldFileName = "../resources/Map/" + mapFileName + ".json";
        if (std::filesystem::exists(oldFileName)) {
            std::filesystem::remove(oldFileName);
        }
        updateListMapFile(mapFileName, cleanName);
    }
    std::ofstream file("../resources/Map/" + cleanName + ".json");
    if (!file.is_open()) {
        std::cerr << "Failed to open map file for saving: " << cleanName << std::endl;
        return;
    }
    file << "{\n";
    file << "    \"width\": " << MAP_WIDTH << ",\n";
    file << "    \"height\": " << MAP_HEIGHT << ",\n";
    file << "    \"layers\": [\n";
    file << "        {\n";
    file << "            \"data\": [";
    for (int y = 0; y < MAP_HEIGHT; ++y) {
        for (int x = 0; x < MAP_WIDTH; ++x) {
            int index = y * MAP_WIDTH + x;
            file << mapGrid[index];
            if (index < (MAP_HEIGHT * MAP_WIDTH) - 1) file << ", ";
            if ((index + 1) % MAP_WIDTH == 0 && index < (MAP_HEIGHT * MAP_WIDTH) - 1) file << "\n                ";
        }
    }
    file << "]\n";
    file << "        }\n";
    file << "    ]\n";
    file << "}";
    file.close();
    isSaved = true;
    mapFileName = cleanName;
}

void CustomMapState::loadMap() {
    std::ifstream file("../resources/Map/" + std::string(mapFileName) + ".json");
    if (!file.is_open()) {
        std::cerr << "Failed to open map file for loading: " << mapFileName << std::endl;
        return;
    }
    json j;
    file >> j;
    file.close();
    MAP_WIDTH = j["width"].get<int>();
    MAP_HEIGHT = j["height"].get<int>(); 
    widthBuffer = std::to_string(MAP_WIDTH);
    heightBuffer = std::to_string(MAP_HEIGHT);
    mapNameBuffer = mapFileName;
    mapGrid = j["layers"][0]["data"].get<std::vector<int>>();
}

void CustomMapState::clearMap() {
    std::fill(mapGrid.begin(), mapGrid.end(), 0);
    isSaved = false;
}

void CustomMapState::applyMapSize() {
    if( widthBuffer.empty() || heightBuffer.empty()) return;
    if( stoi(widthBuffer) == MAP_WIDTH && stoi(heightBuffer) == MAP_HEIGHT) return;
    int newWidth = std::stoi(widthBuffer);
    int newHeight = std::stoi(heightBuffer);
        
    if (newWidth <= 0 || newHeight <= 0) return;

    MAP_WIDTH = newWidth;
    MAP_HEIGHT = newHeight;
    mapGrid.assign(MAP_HEIGHT * MAP_WIDTH, 0);
    camera->target = {MAP_WIDTH * TILE_SIZE * 0.5f, MAP_HEIGHT * TILE_SIZE * 0.5f};
        
    int paletteWidth = isPaletteVisible ? PALETTE_WIDTH : 0;
    float availableWidth = GetScreenWidth() - paletteWidth;
    float availableHeight = GetScreenHeight() - TOOLBAR_HEIGHT;
        
    float zoomX = (availableWidth * 0.8f) / (MAP_WIDTH * TILE_SIZE);
    float zoomY = (availableHeight * 0.8f) / (MAP_HEIGHT * TILE_SIZE);
    camera->zoom = std::clamp(std::min(zoomX, zoomY), 0.1f, 5.0f);
    camera->offset = {availableWidth * 0.5f, (availableHeight + TOOLBAR_HEIGHT) * 0.5f};  
    isSaved = false;
}

void CustomMapState::setMap(int width, int height, const std::vector<int>& mapGrid) {
    MAP_WIDTH = width;
    MAP_HEIGHT = height;
    widthBuffer = std::to_string(MAP_WIDTH);
    heightBuffer = std::to_string(MAP_HEIGHT);
    this->mapGrid = mapGrid;
}

void CustomMapState::setIsSaved(bool saved) {
    isSaved = saved;
}

bool CustomMapState::isMapNameExists(const std::string& name) const {
    std::string mapPath = "../resources/Map/" + name + ".json";
    return std::filesystem::exists(mapPath);
}

std::string CustomMapState::generateUniqueMapName(const std::string& baseName) const {
    std::string uniqueName = baseName;
    int counter = 1;
    while (isMapNameExists(uniqueName)) {
        uniqueName = baseName + " (" + std::to_string(counter) + ")";
        counter++;
    }
    return uniqueName;
}

void CustomMapState::updateListMapFile(const std::string& oldName, const std::string& newName, const std::string& fileName) {
    std::ifstream file(fileName);
    json j;
    
    if (file.is_open()) {
        file >> j;
        file.close();
    }
    
    if (j.contains("listMapName") && j["listMapName"].is_array()) {
        auto& mapList = j["listMapName"];
        for (auto& mapName : mapList) {
            if (mapName.is_string() && mapName.get<std::string>() == oldName) {
                mapName = newName;
                break;
            }
        }
    }
    
    std::ofstream outFile(fileName);
    if (outFile.is_open()) {
        outFile << j.dump(4);
        outFile.close();
        std::cout << "Updated map name in ListMap.json: " << oldName << " -> " << newName << std::endl;
    } else {
        std::cerr << "Failed to update ListMap.json" << std::endl;
    }
}

void CustomMapState::saveHistory() {
    CustomMapMemento memento(mapFileName, MAP_WIDTH, MAP_HEIGHT, mapGrid);
    history.addMemento(memento);
}

void CustomMapState::undo() {
    if (history.canUndo()) {
        history.undo();
        CustomMapMemento memento = history.getMemento();
        setMapFileName(memento.getMapFileName());
        setMap(memento.getWidth(), memento.getHeight(), memento.getMapGrid());
    }
}

void CustomMapState::redo() {
    if (history.canRedo()) {
        history.redo();
        CustomMapMemento memento = history.getMemento();
        setMapFileName(memento.getMapFileName());
        setMap(memento.getWidth(), memento.getHeight(), memento.getMapGrid());
    }
}