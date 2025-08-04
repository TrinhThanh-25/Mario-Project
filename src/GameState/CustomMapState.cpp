#include "GameState/CustomMapState.h"
#include "Common/ResourceManager.h"
#include <fstream>
#include <iostream>

CustomMapState::CustomMapState(World* world)
    : GameState(world, GameStateType::CUSTOM_MAP) {
    mapGrid = std::vector<std::vector<int>>(MAP_HEIGHT, std::vector<int>(MAP_WIDTH, 0));
    loadTileTextures();
}

void CustomMapState::enter() {}

void CustomMapState::exit() {}

void CustomMapState::update() {
    Vector2 mouse = GetMousePosition();

    // ----- Drag map khi không chọn tile -----
    if (selectedTileId == 0 && IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
        if (!isPanning) {
            isPanning = true;
            lastMouse = mouse;
        } else {
            panOffset.x += mouse.x - lastMouse.x;
            panOffset.y += mouse.y - lastMouse.y;
            lastMouse = mouse;
        }
    } else {
        isPanning = false;
    }

    // ----- Cập nhật kích thước hiển thị -----
    int paletteWidth = isPaletteVisible ? (paletteCols * (TILE_SIZE + MARGIN) + 20) : 0;
    viewCols = (GetScreenWidth() - paletteWidth - 80) / TILE_SIZE;
    viewRows = (GetScreenHeight() - 100) / TILE_SIZE;

    // Resize map nếu cần
    if ((int)mapGrid[0].size() != MAP_WIDTH) {
        for (auto& row : mapGrid) {
            row.resize(MAP_WIDTH, 0);
        }
    }

    // ----- Rê chuột trái để vẽ tile nếu đang chọn -----
    if (selectedTileId > 0 && IsMouseButtonDown(MOUSE_LEFT_BUTTON) && !isPanning) {
        int x = (mouse.x - mapOffset.x - panOffset.x) / TILE_SIZE;
        int y = (mouse.y - mapOffset.y - panOffset.y) / TILE_SIZE;

        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
            mapGrid[y][x] = isEraseMode ? 0 : selectedTileId;
        }
    }

    // ----- Palette offset -----
    int paletteOffsetX = GetScreenWidth() - (paletteCols * (TILE_SIZE + MARGIN)) - 10;
    int paletteY = 80;
    int paletteRows = (GetScreenHeight() - paletteY - 40) / (TILE_SIZE + MARGIN);

    // ----- Chuẩn bị danh sách tile -----
    std::vector<int> drawList;
    if (selectedCategory == "ALL") {
        for (int i = 1; i <= (int)tileIds.size(); ++i) {
            drawList.push_back(i);
        }
    } else {
        drawList = tileCategories[selectedCategory];
    }

    // ----- Scroll tile palette -----
    if (GetMouseWheelMove() < 0 || IsKeyPressed(KEY_DOWN)) {
        if ((paletteScrollOffset + paletteRows) * paletteCols < (int)drawList.size()) {
            paletteScrollOffset++;
        }
    }
    if (GetMouseWheelMove() > 0 || IsKeyPressed(KEY_UP)) {
        if (paletteScrollOffset > 0) {
            paletteScrollOffset--;
        }
    }

    // ----- Chọn tile từ tile palette -----
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        bool clickedOnTile = false;

        for (int i = 0; i < drawList.size(); ++i) {
            int col = i % paletteCols;
            int row = i / paletteCols;

            if (row < paletteScrollOffset || row >= paletteScrollOffset + paletteRows) continue;

            int x = paletteOffsetX + col * (TILE_SIZE + MARGIN);
            int y = paletteY + (row - paletteScrollOffset) * (TILE_SIZE + MARGIN);

            Rectangle rect = { (float)x, (float)y, (float)TILE_SIZE, (float)TILE_SIZE };
            if (CheckCollisionPointRec(mouse, rect)) {
                int clickedId = drawList[i];
                if (selectedTileId == clickedId) {
                    selectedTileId = 0; // Bỏ chọn nếu click lại
                } else {
                    selectedTileId = clickedId;
                    isEraseMode = false;
                }
                clickedOnTile = true;
                break;
            }
        }

        // Nếu không click vào tile nào → kiểm tra map
        if (!clickedOnTile) {
            int x = (mouse.x - mapOffset.x - panOffset.x) / TILE_SIZE;
            int y = (mouse.y - mapOffset.y - panOffset.y) / TILE_SIZE;

            if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
                selectedTileId = 0; // Click vùng trống → bỏ chọn
            }
        }
    }

    // ----- Các nút chức năng -----
    int buttonY = GetScreenHeight() - 50;
    int buttonX = 60;

    if (drawButton(buttonX, buttonY, 100, 35, "SAVE", 18, (Color){200, 255, 200, 255})) {
        //saveMapToJson("custom_map.json");
    }
    if (drawButton(buttonX + 110, buttonY, 100, 35, "CLEAR", 18, (Color){255, 200, 200, 255})) {
        for (auto& row : mapGrid) std::fill(row.begin(), row.end(), 0);
    }
    if (drawButton(buttonX + 220, buttonY, 100, 35, "LOAD", 18, (Color){200, 200, 255, 255})) {
        // TODO: implement load
    }
    if (drawButton(buttonX + 330, buttonY, 100, 35, isEraseMode ? "PAINT" : "ERASE", 18, (Color){255, 255, 200, 255})) {
        isEraseMode = !isEraseMode;
    }

    // ----- Tabs tile category -----
    int tabX = GetScreenWidth() - (paletteCols * (TILE_SIZE + MARGIN)) - 80;
    int tabY = 50;
    int buttonWidth = 65;
    int buttonSpacing = 10;

    if (drawButton(tabX + 0, tabY, buttonWidth, 30, "ALL", 16, (Color){220, 220, 220, 255})) selectedCategory = "ALL";
    if (drawButton(tabX + buttonWidth + buttonSpacing, tabY, buttonWidth, 30, "ENEMY", 16, (Color){220, 220, 220, 255})) selectedCategory = "ENEMY";
    if (drawButton(tabX + 2 * (buttonWidth + buttonSpacing), tabY, buttonWidth, 30, "TILE", 16, (Color){220, 220, 220, 255})) selectedCategory = "TILE";
    if (drawButton(tabX + 3 * (buttonWidth + buttonSpacing), tabY, buttonWidth, 30, "BLOCK", 16, (Color){220, 220, 220, 255})) selectedCategory = "BLOCK";
    if (drawButton(tabX + 4 * (buttonWidth + buttonSpacing), tabY, buttonWidth, 30, "ITEM", 16, (Color){220, 220, 220, 255})) selectedCategory = "ITEM";

    // ----- Toggle palette -----
    int toggleBtnX = GetScreenWidth() - 50;
    int toggleBtnY = 50;

    if (drawButton(toggleBtnX, toggleBtnY, 40, 40, isPaletteVisible ? "<<" : ">>", 20, (Color){200, 200, 255, 255})) {
        isPaletteVisible = !isPaletteVisible;
    }
}



void CustomMapState::draw() {
    ClearBackground((Color){230, 230, 230, 255});
    drawMapWithOffset();
    drawTilePalette();
}

void CustomMapState::drawMapWithOffset() const {
    int offsetX = (int)mapOffset.x;
    int offsetY = (int)mapOffset.y;

    // Vẽ nền vùng bản đồ
    DrawRectangle(offsetX - 2, offsetY - 2, viewCols * TILE_SIZE + 4, viewRows * TILE_SIZE + 4, (Color){245, 245, 245, 255});
    DrawRectangleLines(offsetX - 2, offsetY - 2, viewCols * TILE_SIZE + 4, viewRows * TILE_SIZE + 4, DARKGRAY);

    int tileOffsetX = (int)(panOffset.x) % TILE_SIZE;
    int tileOffsetY = (int)(panOffset.y) % TILE_SIZE;

    int startX = (int)(-panOffset.x) / TILE_SIZE;
    int startY = (int)(-panOffset.y) / TILE_SIZE;

    for (int y = 0; y < viewRows; ++y) {
        int mapY = y + startY;
        if (mapY < 0 || mapY >= MAP_HEIGHT) continue;

        for (int x = 0; x < viewCols; ++x) {
            int mapX = x + startX;
            if (mapX < 0 || mapX >= MAP_WIDTH) continue;

            int tileId = mapGrid[mapY][mapX];
            if (tileId > 0 && tileId <= (int)tileIds.size()) {
                const std::string& key = tileIds[tileId - 1];
                Texture2D tex = ResourceManager::getTexture()[key];

                int drawX = offsetX + x * TILE_SIZE + tileOffsetX;
                int drawY = offsetY + y * TILE_SIZE + tileOffsetY + TILE_SIZE - tex.height;

                DrawTexture(tex, drawX, drawY, WHITE);
            }
        }
    }

    // Vẽ lưới
    for (int x = 0; x <= viewCols; ++x) {
        int xPos = offsetX + x * TILE_SIZE + tileOffsetX;
        DrawLine(xPos, offsetY, xPos, offsetY + viewRows * TILE_SIZE, LIGHTGRAY);
    }

    for (int y = 0; y <= viewRows; ++y) {
        int yPos = offsetY + y * TILE_SIZE + tileOffsetY;
        DrawLine(offsetX, yPos, offsetX + viewCols * TILE_SIZE, yPos, LIGHTGRAY);
    }
}

void CustomMapState::drawTilePalette() const {
    if (!isPaletteVisible) return;

    int screenWidth = GetScreenWidth();
    int paletteOffsetX = screenWidth - (paletteCols * (TILE_SIZE + MARGIN)) - 10;
    int paletteY = 80;
    int screenHeight = GetScreenHeight();
    int paletteRows = (screenHeight - paletteY - 40) / (TILE_SIZE + MARGIN);

    DrawText("TILE PALETTE", paletteOffsetX, 20, 20, DARKGRAY);

    std::vector<int> drawList;
    if (selectedCategory == "ALL") {
        for (int i = 1; i < (int)tileIds.size(); i++) {
            drawList.push_back(i);
        }
    } else {
        drawList = tileCategories.at(selectedCategory);
    }

    for (int i = 0; i < drawList.size(); i++) {
        int col = i % paletteCols;
        int row = i / paletteCols;

        Rectangle dest = {
            (float)(paletteOffsetX + col * (TILE_SIZE + MARGIN)),
            (float)(paletteY + row * (TILE_SIZE + MARGIN)),
            (float)TILE_SIZE,
            (float)TILE_SIZE
        };

        int tileId = drawList[i];
        const std::string& key = tileIds[tileId - 1];
        Texture2D tex = ResourceManager::getTexture()[key];

        Color tint = (selectedTileId == tileId) ? RED : WHITE;

        DrawTexturePro(tex, {0, 0, (float)tex.width, (float)tex.height}, dest, {0, 0}, 0.0f, tint);
        DrawRectangleLines(dest.x, dest.y, TILE_SIZE, TILE_SIZE, GRAY);
    }

    DrawText("Use ↑/↓ or mouse wheel to scroll", paletteOffsetX, paletteY + paletteRows * (TILE_SIZE + MARGIN) + 5, 16, DARKGRAY);
}

bool CustomMapState::drawButton(int x, int y, int w, int h, const char* label, int fontSize, Color hoverColor) const {
    Rectangle rect = { (float)x, (float)y, (float)w, (float)h };
    Vector2 mouse = GetMousePosition();
    bool isHovered = CheckCollisionPointRec(mouse, rect);

    Color backgroundColor = isHovered ? hoverColor : LIGHTGRAY;
    Color borderColor = isHovered ? DARKGRAY : GRAY;

    DrawRectangleRec(rect, backgroundColor);
    DrawRectangleLinesEx(rect, 2, borderColor);
    DrawText(label, x + 10, y + (h - fontSize) / 2, fontSize, BLACK);

    return (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && isHovered);
}

void CustomMapState::loadTileTextures() {
    for (int i = 1; i <= 149; ++i) {
        tileIds.push_back("Tileset" + std::to_string(i));
        if (i >= 2 && i <= 107){
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