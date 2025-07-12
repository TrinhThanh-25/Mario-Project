#ifndef SAVEGAME_H
#define SAVEGAME_H

#include "json.hpp"
#include "Game/World.h"
#include <string>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;

class SaveGame {
    public:
        static void saveGame(const World& world, const std::string& filename = "../SaveGame.json");
        static void loadGame(World& world, const std::string& filename = "../SaveGame.json");
        static void deleteSaveGame(const std::string& filename = "../SaveGame.json");
        static bool saveGameExists(const std::string& filename = "../SaveGame.json");
};

#endif