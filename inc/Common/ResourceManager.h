#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "raylib.h"
#include <string>
#include <unordered_map>
#include <vector>

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

        static void drawWhiteSmallNumber( int number, int x, int y );
        static void drawYellowSmallNumber( int number, int x, int y );
        static void drawBigNumber( int number, int x, int y );
        static int getSmallNumberWidth( int number );
        static int getSmallNumberHeight();
        static int getBigNumberWidth( int number );
        static int getBigNumberHeight();

        static void drawString( std::string str, int x, int y );
        static void drawBigString( std::string str, int x, int y , int size = 20 );
        static int getDrawStringWidth( std::string str );
        static int getDrawStringHeight();
        static int getDrawBigStringWidth( std::string str, int size = 20 );
        static int getDrawBigStringHeight(int size = 20);
        static void drawMessageString( std::string str, int x, int y );
        static int getMessageStringWidth( std::string str );
        static int getMessageStringHeight();

        static std::vector<std::string> split( const std::string& s, char delim = '\n' );
};

#endif