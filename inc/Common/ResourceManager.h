#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "raylib.h"
#include <string>
#include <unordered_map>

class ResourceManager {
    private:
        static std::unordered_map<std::string, Texture2D> texture;
        static std::unordered_map<std::string, Sound> sound;
        static std::unordered_map<std::string, Music> music;

        static void loadTexture();
        static void loadSound();
        static void loadMusic();

        static void unloadTexture();
        static void unloadSound();
        static void unloadMusic();

        static Texture2D texture2DFlipHorizontal(Texture2D texture);
    public:
        static std::unordered_map<std::string, Texture2D>& getTexture();
        static std::unordered_map<std::string, Sound>& getSound();
        static std::unordered_map<std::string, Music>& getMusic();

        static void loadResources();
        static void unloadResources();
};

#endif