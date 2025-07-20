#include "SaveGame.h"
#include <iostream>

void SaveGame::saveGame(const World& world, const std::string& filename) {
    json j = world.saveToJson();
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Could not open file " << filename << " for saving." << std::endl;
        return;
    }
    file << j.dump(4);
    if (!file) {
        std::cerr << "Error writing to file " << filename << std::endl;
    } else {
        std::cout << "Game saved successfully to " << filename << std::endl;
    }
    file.close();
}

void SaveGame::loadGame(World& world, const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        std::cerr << "Save game file does not exist: " << filename << std::endl;
        return;
    }
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Could not open file " << filename << " for loading." << std::endl;
        return;
    }
    json j;
    file >> j;
    world.loadFromJson(j);
}

void SaveGame::deleteSaveGame(const std::string& filename) {
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
}

bool SaveGame::saveGameExists(const std::string& filename) {
    return std::filesystem::exists(filename);
}