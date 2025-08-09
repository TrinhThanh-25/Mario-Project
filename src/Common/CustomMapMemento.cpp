#include "Common/CustomMapMemento.h"

CustomMapMemento::CustomMapMemento(const std::string& mapFileName, int width, int height, const std::vector<int>& mapGrid)
    : mapFileName(mapFileName), width(width), height(height), mapGrid(mapGrid) {}

std::string CustomMapMemento::getMapFileName() const {
    return mapFileName;
}

int CustomMapMemento::getWidth() const {
    return width;
}

int CustomMapMemento::getHeight() const {
    return height;
}

const std::vector<int>& CustomMapMemento::getMapGrid() const {
    return mapGrid;
}