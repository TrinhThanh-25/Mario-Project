#ifndef WORLD_H
#define WORLD_H

#include "Mario/Mario.h"
#include "Common/Drawable.h"
#include "Game/Map.h"
#include <vector>
#include <string>

class World : virtual public Drawable {
    private:
        Mario mario;
        Map map;
        Camera2D camera;
        
        //Window
        int width;
        int height;
        std::string title;
        int FPS;
        
    public:
        static float gravity;
    public:
        World(int width, int height, const std::string& title, int FPS);
        ~World() override;
        
        void init();

        void update();
        void draw() override;

        void setCamera(Camera2D *camera);
        const Camera2D* getCamera() const;
};

#endif